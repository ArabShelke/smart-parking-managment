
from flask import Flask, jsonify
from flask_cors import CORS
import ctypes
import os
from datetime import datetime
c_library = ctypes.CDLL(
    os.path.join(os.path.dirname(__file__), "parking.dll")
)

c_library.getTotalSlots.restype = ctypes.c_int
c_library.initializeSlots.argtypes = []
c_library.initializeSlots.restype = None
c_library.initializeSlots()

c_library.initializeGraph.argtypes = []
c_library.initializeGraph.restype = None
c_library.initializeGraph()

c_library.setSlotVehicleNumber.argtypes = [
    ctypes.c_int,
    ctypes.c_char_p
]
c_library.setSlotVehicleNumber.restype = None

c_library.getSlotVehicleNumber.argtypes = [ctypes.c_int]
c_library.getSlotVehicleNumber.restype = ctypes.c_char_p

c_library.calculateFee.argtypes = [
    ctypes.c_int, ctypes.c_int,
    ctypes.c_int, ctypes.c_int
]
c_library.calculateFee.restype = ctypes.c_float

c_library.saveHistory.argtypes = [
    ctypes.c_char_p, ctypes.c_char_p,
    ctypes.c_int,
    ctypes.c_int, ctypes.c_int,
    ctypes.c_int, ctypes.c_int,
    ctypes.c_float
]
c_library.saveHistory.restype = None



app = Flask(__name__)
CORS(app)
entry_times = {}

@app.route("/")
def home():
    return "Smart Parking Backend is Running!"


@app.route("/api/slots")
def get_slots():
    slots = []

    for i in range(1, 11):
        occupied = c_library.getSlotOccupied(i)

        slots.append({
            "number": i,
            "type": "Car" if i <= 5 else "Bike",
            "occupied": bool(occupied),
            "vehicleNumber": c_library.getSlotVehicleNumber(i).decode("utf-8")
        })

    return jsonify(slots)

@app.route("/api/nearest-slot/<vehicle_type>")
def nearest_slot(vehicle_type):
    vehicle_type = vehicle_type.capitalize()

    if vehicle_type not in ["Car", "Bike"]:
        return jsonify({"error": "Invalid vehicle type"}), 400

    c_library.findNearestSlotByType.argtypes = [ctypes.c_char_p]
    c_library.findNearestSlotByType.restype = ctypes.c_int

    slot = c_library.findNearestSlotByType(
        vehicle_type.encode("utf-8")
    )

    if slot == -1:
        return jsonify({"message": "No available slot"}), 404

    return jsonify({
        "vehicleType": vehicle_type,
        "nearestSlot": slot
    })


@app.route("/api/occupy-slot/<int:slot_number>", methods=["POST"])
def occupy_slot(slot_number):
    from flask import request

    if slot_number < 1 or slot_number > 10:
        return jsonify({"error": "Invalid slot number"}), 400

    data = request.get_json(silent=True) or {}
    vehicle_number = data.get("vehicleNumber", "").strip().upper()

    if not vehicle_number:
        return jsonify({"error": "Vehicle number required"}), 400

    c_library.isSlotAvailable.argtypes = [ctypes.c_int]
    c_library.isSlotAvailable.restype = ctypes.c_int

    if not c_library.isSlotAvailable(slot_number):
        return jsonify({"error": "Slot is already occupied"}), 409

    c_library.updateSlotStatus.argtypes = [ctypes.c_int, ctypes.c_int]
    c_library.updateSlotStatus.restype = None
    c_library.updateSlotStatus(slot_number, 1)

    c_library.setSlotVehicleNumber(
        slot_number, vehicle_number.encode("utf-8")
    )
    entry_times[vehicle_number] = datetime.now()

    return jsonify({
        "message": "Slot occupied successfully",
        "slotNumber": slot_number,
        "vehicleNumber": vehicle_number
    })



@app.route("/api/exit-vehicle", methods=["POST"])
def exit_vehicle():
    from flask import request

    data = request.get_json(silent=True) or {}
    vehicle_number = data.get("vehicleNumber", "").strip().upper()

    if not vehicle_number:
        return jsonify({"error": "Vehicle number required"}), 400

    for slot_number in range(1, 11):
        saved = c_library.getSlotVehicleNumber(slot_number)
        saved_number = saved.decode("utf-8") if saved else ""

        if saved_number == vehicle_number:
            entry_time = entry_times.pop(vehicle_number, None)
            exit_time = datetime.now()
            fee = 0.0

            if entry_time is not None:
                fee = c_library.calculateFee(
                    entry_time.hour, entry_time.minute,
                    exit_time.hour, exit_time.minute
                )

                c_library.saveHistory(
                    vehicle_number.encode("utf-8"),
                    ("Car" if slot_number <= 5 else "Bike").encode("utf-8"),
                    slot_number,
                    entry_time.hour, entry_time.minute,
                    exit_time.hour, exit_time.minute,
                    fee
                )

            c_library.updateSlotStatus(slot_number, 0)
            c_library.setSlotVehicleNumber(slot_number, b"")

            return jsonify({
                "message": "Vehicle exited successfully",
                "slotNumber": slot_number,
                "vehicleNumber": vehicle_number,
                "fee": round(fee, 2)
            })

    return jsonify({"error": "Vehicle not found"}), 404




@app.route("/api/c-test")
def c_test():
    total = c_library.getTotalSlots()
    return jsonify({"totalSlotsFromC": total})
if __name__ == "__main__":
    app.run(debug=True)
