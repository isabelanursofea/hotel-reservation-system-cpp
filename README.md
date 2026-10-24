# Hotel Reservation System

A C++ console application for booking rooms, ordering room service and facilities, and paying, with login and receipts. Built as a group project for **CSC402 Programming** (Group 5, Faculty of Computer and Mathematical Sciences, UiTM).

## Features
- User sign up and login
- Browse and book hotel rooms
- Order room service (food and beverages, housekeeping, entertainment)
- Choose hotel facilities (gym, spa, bicycle rental) and event reservations
- Payment with a choice of payment methods and card detail checks
- Reservation summary and overall receipt, with the total price calculated
- Coloured console menus (ANSI colours) and background music

## Tech Stack
- C++ (Windows console)
- `windows.h` and `MMSystem.h` (`PlaySound`) for the background music
- Developed in Dev-C++

## Files
- `hotel-reservation.cpp`: the full source code
- `coffee.wav`: background music (a 60-second clip)
- `modules/new ROOM SERVICE.cpp`: the Room Service module on its own
- `docs/`: the group proposal (PDF)
- `screenshots/`: login, sign up, menu, room booking, payment and receipt screens

## How to Run
1. Install **Dev-C++** (or any C++ compiler on Windows).
2. Open `hotel-reservation.cpp` and compile it. If the compiler asks for it, add `-lwinmm` to the linker options.
3. Keep `coffee.wav` in the **same folder** as the program so the music can play.
4. Run the program and sign up for a new user.
5. Follow the menu to book a room, add services and pay.

## Notes
- This is a console program for Windows. It will not compile on Mac or Linux without changes.
- If `coffee.wav` is missing, the program still runs, but without music.
- All names and sample details in the screenshots are for demonstration only.

## My Contribution
I developed the **Room Service** module and added the **background music**.

**Room Service module** (`modules/new ROOM SERVICE.cpp`) for the "Del Luna" hotel:
- A menu with three services: Food and Beverages, Housekeeping and Entertainment
- **Food and Beverages:** guests pick one food item and one drink from lists of five each, and the program checks the choice, calculates the cost and adds it to the running total
- **Housekeeping:** guests enter a room number to request cleaning, and the program confirms when it's done
- **Entertainment:** guests choose Movies, TV Shows, Music or Games and receive an access code
- A total price summary when the guest exits the menu, with input validation for invalid choices
- Console design: coloured text, a welcome banner, an animated menu title, a loading-dots effect and an exit countdown

**Background music:** I added the background music using the Windows `PlaySound` function, which plays `coffee.wav` when the program starts. The clip is trimmed to 60 seconds and converted to a smaller WAV so it fits on GitHub.
