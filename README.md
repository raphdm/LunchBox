# LunchBox - Student Toolkit

## Description

LunchBox is a console-based C++ application designed as a toolkit for students. It provides various features to help manage academic information, tasks, notes, and citations. The application uses a modular design, allowing for potential future expansion. User data is saved locally in the `SavedData` directory.

## Features

The application includes the following modules:

* **Authentication**: Handles user registration and login to secure access to the toolkit. User credentials are saved locally.
  
* **Academic Records**:
  
    * **Student Profile Management**: Allows users to add, edit, view, and delete their basic student information (Full Name, Student ID, Course, Year Level). Profile data is saved per user.
    * **Grade Tracker**: Enables users to manage semesters, add subjects, track grades per term, calculate averages, and view grade reports[cite: 7, 9]. Grade data is saved per user.
      
* **Kanban Board**: A task management system where users can:
    * Add, update, and delete tasks with titles, descriptions, and due dates.
    * Move tasks between "To Do", "In Progress", and "Done" statuses.
    * View tasks in a column-based board.
    * Search for specific tasks.
    * Task data is saved per user.
      
* **Citation Builder**: Helps users create citations for various sources:
    * Supports APA, MLA, and Chicago citation formats.
    * Handles source types like Books, Articles, Websites, and Research Papers.
    * Saves citation history per user.
      
* **Note Taking**: A simple note-taking feature:
    * Create, view, edit, list, search, and delete notes.
    * Notes have a title, content, creation date, and last modified date.
    * Notes are saved as individual text files per user.

## Getting Started

### Prerequisites

* A C++ compiler that supports C++11 or later.
* Windows Operating System (due to Windows-specific headers like `<windows.h>` and `<direct.h>` being used).

### Building

1.  Compile all the `.cpp` files located in the `src` directory and its subdirectories (`AcademicRecords`, `CitationBuilder`, `Kanban`, `NoteTaking`).
2.  Link the compiled object files together to create the executable (e.g., `LunchBox.exe`).

### Running

1.  Execute the compiled `LunchBox.exe` file from your terminal or command prompt.
2.  The application will first present a login/register menu.
3.  After logging in, the main menu will display the available feature modules.

## Dependencies

* C++ Standard Library (iostream, string, vector, fstream, limits, sstream, algorithm, chrono, etc.)
* Windows API (for console clearing, directory creation, file listing, etc. - e.g., `<windows.h>`, `<direct.h>`)

## License
