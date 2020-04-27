<h1 align="center">ft_select</h1>
<p align="center">
    <img width="" height="80" src="https://github.com/salahadawi/ft_select/blob/master/images/ft_select-video.gif">
</p>
The goal of this project was to create a user interface for a terminal program, using the termcaps library to configure and manipulate the terminal.

This program takes one or several parameters, displays them in the terminal, and allows the user to select through an user interface which parameters are returned to the shell.

## Usage
Launching ft_select with one or more parameters brings up a user interface with these parameters.

* Left arrow: Move cursor left
* Right arrow: Move cursor right
* Up arrow: Jump cursor to start
* Down arrow: Jump cursor to end
* Space: Toggle selection
* '+' key: Select all
* '-' key: Deselect all
* Enter: Exit program and return selected parameters
* Escape: Cancel all selections and exit program

## Example
Launcing ft_select in my workspace like this:
```
./ft_select *
```
Results in this:

<img width="" height="80" src="https://github.com/salahadawi/ft_select/blob/master/images/ft_select.png">

Selecting some parameters:

<img width="" height="80" src="https://github.com/salahadawi/ft_select/blob/master/images/ft_select-selections.png">

Pressing enter to return selected parameters:

<img width="" height="80" src="https://github.com/salahadawi/ft_select/blob/master/images/ft_select-return.png">

The selected parameters are displayed.
