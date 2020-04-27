# ft_select - Terminal manipulation with termcaps.
The goal of this project was to create a user interface for a terminal program, using the termcaps library to configure and manipulate the terminal.

## Usage
Launching ft_select with one or more parameters brings up a user interface with these parameters.

* Left arrow: Move left
* Right arrow: Move right
* Up arrow: Jump to start
* Down arrow: Jump to end
* Space: Toggle selection
* '+' key: Select all
* '-' key: Deselect all
* Enter: Exit program and return selected parameters
* Escape: Cancel all selections and exit program

Launcing ft_select in my workspace like this:
```
./ft_select *
```
Results in this:
