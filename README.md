# Smart Parking Management System

A Smart Parking Management System developed using C, Python Flask, HTML, CSS, and JavaScript. The project manages vehicle entry and exit, parking slot allocation, vehicle records, and parking fee calculation through a web-based interface.

## Overview

The system provides a digital approach to parking management by combining C-based core logic with a Flask backend and an interactive frontend.

## Key Features

- Real-time parking slot management
- Separate parking allocation for cars and bikes
- Vehicle entry and exit management
- Duplicate vehicle detection
- Vehicle search functionality
- Automated parking fee calculation
- Parking history management
- C library integration with Python using ctypes
- Interactive web-based dashboard

## Technology Stack

| Technology | Purpose |
|---|---|
| C | Core parking logic |
| Python | Backend integration |
| Flask | Web application and API |
| HTML | Web structure |
| CSS | Styling and layout |
| JavaScript | Frontend functionality |
| ctypes | C and Python integration |

## System Architecture

1. **Frontend:** HTML, CSS, and JavaScript provide the user interface.
2. **Backend:** Python Flask handles application logic and API requests.
3. **Core Engine:** C handles parking operations and calculations.
4. **Integration:** Python ctypes connects the Flask backend with the compiled C library.

## Parking Configuration

- Total Parking Slots: 10
- Car Slots: 5
- Bike Slots: 5
- Parking Fee: ₹30 per hour

## Project Structure

```text
smart-parking-management/
│
├── app.py
├── index.html
├── style.css
├── script.js
│
├── main.c
├── parking.c
├── parking.h
├── parking.dll
│
├── allocation.c
├── allocation.h
├── billing.c
├── billing.h
├── vehicle.c
├── vehicle.h
│
└── history.txt
```

## Testing

The following functionalities were tested:

- Dashboard slot count
- Car and bike entry
- Duplicate vehicle detection
- Vehicle search
- Vehicle exit
- Parking history
- Page refresh functionality
- C library integration with Flask

## Learning Outcomes

- Practical implementation of C programming concepts
- Integration of C with Python using ctypes
- Backend development using Flask
- Frontend and backend communication
- Working with APIs and application logic
- Building and testing a complete software project

## Future Improvements

- Database integration
- User authentication
- Online parking reservations
- Payment gateway integration
- Live parking availability updates

## Author

**Arab Raosaheb Shelke**

B.Tech CSE (AI/ML) Student

GitHub: [@ArabShelke](https://github.com/ArabShelke)

---

*Developed as an academic software development project.*
