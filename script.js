// Parking slots


let savedSlots = JSON.parse(localStorage.getItem("parkingSlots"));

let slots = [];

for (let i = 1; i <= 10; i++) {
    let savedSlot = Array.isArray(savedSlots)
        ? savedSlots.find(slot => slot.number === i)
        : null;

    slots.push({
        number: i,
        type: i <= 5 ? "Car" : "Bike",
        occupied: savedSlot ? savedSlot.occupied : false,
        vehicleNumber: savedSlot ? (savedSlot.vehicleNumber || "") : ""
    });
}

localStorage.setItem("parkingSlots", JSON.stringify(slots));
let parkingHistory =
    JSON.parse(localStorage.getItem("parkingHistory")) || [];


// Show selected section
function showSection(sectionId) {

    const sections = document.querySelectorAll(".section");

    sections.forEach(section => {
        section.classList.remove("active");
    });

    document.getElementById(sectionId).classList.add("active");
    
document.querySelectorAll(".nav-btn").forEach(button => {
    button.classList.remove("active");
});

const activeButton = document.querySelector(
    `.nav-btn[onclick="showSection('${sectionId}')"]`
);

if (activeButton) {
    activeButton.classList.add("active");
}


    updateDashboard();
    displaySlots();
    if (sectionId === "history") {
    displayHistory();
}
}


// Update dashboard
function updateDashboard() {

    let occupied = 0;

    slots.forEach(slot => {
        if (slot.occupied) {
            occupied++;
        }
    });

    let available = slots.length - occupied;
    
const percentage = Math.round(
    (occupied / slots.length) * 100
);

document.getElementById("occupancyBar").style.width =
    percentage + "%";

document.getElementById("occupancyText").textContent =
    percentage + "% Occupied";


    document.getElementById("totalSlots").textContent = slots.length;
    document.getElementById("availableSlots").textContent = available;
    document.getElementById("occupiedSlots").textContent = occupied;
}


// Display parking slots
function displaySlots() {

    const container = document.getElementById("slotContainer");

    container.innerHTML = "";

    slots.forEach(slot => {

        const slotElement = document.createElement("div");

        slotElement.classList.add("slot");

        if (slot.occupied) {

            slotElement.classList.add("occupied");

            
slotElement.innerHTML =
    "🚗 Slot " + slot.number +
    "<br>Occupied" +
    "<br>" + slot.vehicleNumber;


        } else {

            slotElement.classList.add("available");

            
    slotElement.innerHTML =
        (slot.type === "Car" ? "🚗 " : "🏍️ ") +
        "Slot " + slot.number +
        "<br>Available" +
        "<br>" + slot.type;

    }

        container.appendChild(slotElement);
    });
}



// Park vehicle

document.getElementById("parkingForm").addEventListener("submit", async function(event) {
    event.preventDefault();

    const vehicleNumber = document
        .getElementById("vehicleNumber").value.trim().toUpperCase();

    const vehicleType = document.getElementById("vehicleType").value;
    const message = document.getElementById("parkingMessage");

    if (!vehicleNumber) {
        message.textContent = "Please enter vehicle number.";
        return;
    }

    try {
        const nearestResponse = await fetch(
            `http://127.0.0.1:5000/api/nearest-slot/${encodeURIComponent(vehicleType)}`
        );
        const nearestData = await nearestResponse.json();

        if (!nearestResponse.ok) {
            message.textContent = nearestData.message || "No slot available.";
            return;
        }

        const response = await fetch(
            `http://127.0.0.1:5000/api/occupy-slot/${nearestData.nearestSlot}`,
            {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ vehicleNumber })
            }
        );

        const data = await response.json();

        if (!response.ok) {
            message.textContent = data.error || "Parking failed.";
            return;
        }

        message.textContent =
            `${vehicleNumber} parked in Slot ${data.slotNumber}.`;

        document.getElementById("parkingForm").reset();

        await loadSlotsFromBackend();

    } catch (error) {
        console.error(error);
        message.textContent = "Backend error. Check Flask server.";
    }
});


 // Vehicle exit
document.getElementById("exitForm").addEventListener("submit", async function(event) {
    event.preventDefault();

    const vehicleNumber = document
        .getElementById("exitVehicleNumber")
        .value.trim().toUpperCase();

    const message = document.getElementById("exitMessage");

    try {
        const response = await fetch(
            "http://127.0.0.1:5000/api/exit-vehicle",
            {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({ vehicleNumber })
            }
        );

        const data = await response.json();

        if (!response.ok) {
            message.textContent = data.error || "Vehicle not found!";
            return;
        }

        // Add exit record to history
        const exitedSlot = slots.find(
            slot => slot.number === data.slotNumber
        );

        parkingHistory.push({
            vehicleNumber: vehicleNumber,
            vehicleType: exitedSlot ? exitedSlot.type : "Unknown",
            slotNumber: data.slotNumber,
            exitTime: new Date().toLocaleString()
        });

        localStorage.setItem(
            "parkingHistory",
            JSON.stringify(parkingHistory)
        );

        message.textContent =
            `${vehicleNumber} exited from Slot ${data.slotNumber}.`;

        document.getElementById("exitForm").reset();

        await loadSlotsFromBackend();
        displayHistory();

    } catch (error) {
        console.error(error);
        message.textContent = "Backend error. Check Flask server.";
    }
});





// Initial display
updateDashboard();
displaySlots();

function displayHistory() {
    const container = document.getElementById("historyContainer");

    if (parkingHistory.length === 0) {
        container.innerHTML = "<p>No parking history available.</p>";
        return;
    }

    container.innerHTML = "";

    parkingHistory.forEach(record => {
        const item = document.createElement("div");
        item.className = "card";

        item.innerHTML = `
            <h3>${record.vehicleNumber}</h3>
            <p>Type: ${record.vehicleType}</p>
            <p>Slot: ${record.slotNumber}</p>
            <p>Exit: ${record.exitTime}</p>
        `;

        container.appendChild(item);
    });
}

document.getElementById("searchForm").addEventListener("submit", function(event) {
    event.preventDefault();

    const number = document
        .getElementById("searchVehicleNumber")
        .value.trim()
        .toUpperCase();

    const vehicle = slots.find(slot =>
        slot.occupied &&
        slot.vehicleNumber.toUpperCase() === number
    );

    const message = document.getElementById("searchMessage");

    if (vehicle) {
        message.textContent =
            "Vehicle found! Type: " + vehicle.type +
            ", Slot: " + vehicle.number;
    } else {
        message.textContent = "Vehicle not found in parking.";
    }
});

function calculateParkingFee() {
    const entryValue = document.getElementById("entryTime").value;
    const exitValue = document.getElementById("exitTime").value;
    const result = document.getElementById("feeResult");

    if (!entryValue || !exitValue) {
        result.textContent = "Please enter both times.";
        return;
    }

    const entry = new Date(entryValue);
    const exit = new Date(exitValue);

    const duration = (exit - entry) / (1000 * 60 * 60);

    if (duration <= 0) {
        result.textContent = "Exit time must be after entry time.";
        return;
    }

    const fee = duration * 30;

    result.textContent =
        "Duration: " + duration.toFixed(2) +
        " hours | Parking Fee: ₹" + fee.toFixed(2);
}

async function testBackendConnection() {
    try {
        const response = await fetch("http://127.0.0.1:5000/api/slots");

        if (!response.ok) {
            throw new Error("Backend request failed");
        }

        const data = await response.json();

        console.log("Backend connected successfully:", data);

    } catch (error) {
        console.error("Backend connection failed:", error);
    }
}

testBackendConnection();

async function loadSlotsFromBackend() {
    try {
        const response = await fetch(
            "http://127.0.0.1:5000/api/slots"
        );

        if (!response.ok) {
            throw new Error("Could not load parking slots");
        }

        const data = await response.json();

        slots = data;
        displaySlots();
        updateDashboard();
        console.log("Slots received from backend:", data);

    } catch (error) {
        console.error("Could not load slots:", error);
    }
}

loadSlotsFromBackend();

async function parkVehicleUsingBackend(vehicleType) {
    try {
        const nearestResponse = await fetch(
            `http://127.0.0.1:5000/api/nearest-slot/${vehicleType}`
        );

        const nearestData = await nearestResponse.json();

        if (!nearestResponse.ok) {
            alert(nearestData.message || nearestData.error || "No slot available");
            return;
        }

        const occupyResponse = await fetch(
            `http://127.0.0.1:5000/api/occupy-slot/${nearestData.nearestSlot}`,
            
{
    method: "POST",
    headers: {
        "Content-Type": "application/json"
    },
    body: JSON.stringify({
        vehicleNumber: document.getElementById("vehicleNumber").value.trim().toUpperCase()
    })
}

        );

        const occupyData = await occupyResponse.json();

        if (!occupyResponse.ok) {
            alert(occupyData.error || "Could not occupy slot");
            return;
        }

        alert(
            `${vehicleType} allocated to Slot ${occupyData.slotNumber}`
        );

        await loadSlotsFromBackend();

    } catch (error) {
        console.error(error);
        alert("Backend connect nahi hua. Flask server check karo.");
    }
}
