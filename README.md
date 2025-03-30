# Qt-ParachuteEncoder

## Project Overview

ParachuteEncoder is a Qt-based application designed to encode messages into a visual representation inspired by the parachute used during the landing of the Perseverance rover on Mars. The parachute's design contained a hidden binary message, and this project replicates that concept by allowing users to encode their own messages into a parachute-like visualization.

The project follows the Model-View-Presenter (MVP) architectural pattern, ensuring a clear separation of concerns between the data (Model), user interface (View), and logic (Presenter).

## Context and Objective

The idea for this project is inspired by the Perseverance rover's parachute, which carried a hidden binary message during its landing on Mars in February 2021. This project allows users to:

- Encode a custom message into a binary format
- Visualize the binary message as a parachute design with customizable sectors and tracks
- Export the parachute visualization as an image
- Switch between multiple visualization modes (binary view and parachute view)

The application provides an intuitive graphical interface for users to interact with and is available in French.

## Features

- **Message Encoding**:
  - Converts user-provided text into a binary format using ASCII encoding
  - Displays the binary representation in a grid format
  - Supports the standard 7-bit ASCII format with optional 3 additional bits
  - Allows addition of marker bits at the beginning of the message

- **Parachute Visualization**:
  - Visualizes the binary message as a parachute design with customizable sectors and tracks
  - Allows users to customize colors for bits (1 and 0)
  - Supports using random colors for creative visualizations
  - Provides option to leave the center of the parachute empty

- **Binary View**:
  - Displays the binary representation of the encoded message in a grid format
  - Shows the message structure with optional markers and block formatting

- **Export Functionality**:
  - Exports the visualization as an image in various formats (PNG, JPG, BMP)
  - Includes the text message in the exported image for reference

- **Multilingual Support**:
  - Currently supports French language
  - Designed for easy addition of other languages

## Project Structure

The project is organized following the MVP architecture:

```
Projet-Conception/
├── model/                  # Data models
│   ├── messagemodel.*      # Message management and binary conversion
│   └── parachutemodel.*    # Display configuration
├── view/                   # User interfaces
│   ├── binaryview.*        # Binary view
│   ├── parachuteview.h     # Interface for parachute views
│   └── qtparachuteview.*   # Qt implementation of parachute view
├── presenter/              # Presenters
│   └── parachutepresenter.* # Interaction coordination
├── ui/                     # Main interface
│   └── mainwindow.*        # Main application window
├── utils/                  # Utilities
│   ├── Observer.*          # Observer pattern implementation
│   ├── Point.h             # Point structure
│   └── trapezecalcul.*     # Geometric calculations
├── translations/           # Translation files
│   └── parachute_fr.*      # French translation
├── resources.qrc           # Application resources
└── README.md               # This file
```

## How It Works

### Encoding Messages
- Users input a message in the text field
- The message is encoded into binary using ASCII encoding (7 bits per character)
- Optional markers can be added at the beginning of the message
- Optional block formatting adds 3 additional bits per character
- The binary representation is displayed in a grid format in the Binary View

### Parachute Visualization
- The binary message is mapped onto a parachute design
- Users can customize the number of sectors in the parachute
- Users can choose to leave the center of the parachute empty
- The application automatically calculates the number of tracks needed to display the message
- Colors for bits 0 and 1 can be customized or set to random

### Binary View
- Displays the binary message in a grid format
- Shows the structure of the message including any markers
- Provides a clear visual representation of the encoded message

### Exporting
- Users can export the current view (binary or parachute) as an image
- The exported image includes the message text for reference
- Multiple image formats are supported (PNG, JPG, BMP)

## Prerequisites

- Linux or Windows environment
- Qt 5.x
- C++17 compatible compiler

## Building and Running the Project

### Steps to Build and Run

```bash
# Clone the repository
git clone https://github.com/AyGoub/Projet-Conception
cd Projet-Conception

# Configure and build
qmake
make

# Compile translations
lrelease translations/parachute_fr.ts

# Run the application
./Projet-Conception
```

## Using the Application

1. Enter your message in the text field
2. Use the toolbar options to customize the display
3. Switch between binary view and parachute view using the tabs
4. Export the image using File → Export

## Customization Options

- **Colors**: Use the "Color for 1" and "Color for 0" buttons to change the colors
- **Display Options**: 
  - **Markers**: Adds marker bits at the beginning of the message
  - **Block Format**: Adds 3 additional bits per character
  - **Empty Center**: Leaves the center of the parachute empty
  - **Random Colors**: Uses randomly generated colors
- **Number of Sectors**: Modify the number of sectors in the parachute

## Technical Features

- Observer pattern for communication between models and views
- Geometric calculations to generate the trapezoids of the parachute
- Multilingual support with Qt Linguist
- QPainter for graphical rendering

## Troubleshooting

If you encounter issues:
1. Verify that Qt is properly installed
2. Ensure that dependencies are met
3. Try cleaning and rebuilding the project
```bash
make clean
qmake
make
```

## License
This project is licensed under the MIT License. Feel free to use, modify, and distribute the code as needed.

## Credits
Developed by:
- Goubraim Ayoub: ayoub.goubraim@ecole.ensicaen.fr
- Talemsi Saad: saad.talemsi@ecole.ensicaen.fr

Inspired by the coded message in the parachute used by NASA's Perseverance rover. 