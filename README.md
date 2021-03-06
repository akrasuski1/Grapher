# Grapher

This is one of my first ever graphical applications, written in 2013. 
Finding this code in old backups from 6 years ago is quite embarassing... but also a reminder how
much I learned in the meanwhile.

The code features Polish identifiers (despite interface language being English), little to no error checking,
hardcoded constants and, the best of all, handwritten interface. That's right, I wasn't using any graphical
layout editor, but rather changing widget positions manually, recompiling, and checking if it looks okay...
The whole code, for "modularization" is split into two files. Nobody told me you can compile more than one
`cpp` file together, so the main file just includes the other...

Screenshot, as seen running in Wine under Linux:

![screenshot](Screenshot-11.png)

# Original README

Application designed to draw plots of functions and graphically solve differential equations.


1. Installation

Simply download Grapher.exe and Grapher.exe.manifest and put them in one folder. From now on, to open the application, just open the Grapher.exe.

2. Using

After opening, you already have three example functions plotted. To remove them, select them in Functions menu and click on *Remove*. 

To add new functions, click on *Add* in Functions menu. Added functions are defaulted to Constant y=0 function.

To change the formula, select function in Functions menu and use boxes in Function menu to change settings. For example, if you want to add new function f1=2x^2+3x-4, you have to click *Add*, select the new function, then select Quadratic from the combo box on the top, and write coefficients 2, 3, -4 into *A*, *B* and *C* fields. Last thing to do is to apply the settings by pressing *OK*.

In case of the function types like polynomial function, you have to type coefficients separated by spaces. Example input: "1 2 3" yields function y=x^2+2x+3. For function sum or product, you have to write coefficients and operators, for instance: "f1 - f4 + pi" with obvious result.

You can set the visibility of the function on the graph by selecting it and checking or unchecking the checkbox *Show*.

In the coefficients boxes you can write: real numbers (such as 1, 4.5, -2.3e10), "e" as Euler's number, "pi" as the ratio of circle's circumference to its diameter, or finally "fi" (for example "f1", "f6"), to make ith function a coefficient. To change size of graph window you can either: use buttons in bottom right corner, use mouse wheel on graph window, or manually write the needed size in boxes in bottom and apply settings using *Resize*.

To change position of graph window, you can either: drag window using mouse, use buttons in bottom right corner, or manually write the position in boxes in bottom and apply settings using *Resize*.

3. Known bugs

- sometimes the graph window disappears (temporary fix: drag the window)
- differential equations is still blank - not implemented yet
