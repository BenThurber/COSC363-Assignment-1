## Background
For my assignment I designed a model of a hexagonal, one room museum, that the viewer can walk into and view the exhibits. The theme of the museum is based around the inventions of the 19-Century scientist Nikola Tesla. In clockwise order, the first exhibit is a model of the first radio controlled boat. In the back right corner is a Tesla Coil, a device to produce high voltage electricity. To the right of the doorway is an electromechanical oscillator attached to a steel beam, supported at both ends. The oscillator attached to the bar moves up and down, demonstrating the beam’s resonant frequency.

For a more thorough explanation, see README.pdf

## Compilation
The program is configured with a CMakeLists.txt file and cmake. The program is compiled using make.

### Requirements
• C++  
• freeGLUT / GLUT  
• X11  
• cmake  
• make  

### Compile
cd into Assignment1/ directory and run  
`cmake .`
`/ make`  
which will create a binary file called “assignment1”.  

### Run
Run by executing  
`./assignment1`  

## Movement System
The camera is moved through the scene using the keyboard. Two key mappings are available which can be toggled between by pressing shift-t. ↑, ↓, ←, → are the arrow keys.

### Default strict mappings
The following is the default and follows the required key mapping in the assignment.  
• ↑, ↓ Move forward, backward  
• ←, → Look left, right  
• ‘w’, ‘s’ Look up, down  
• ‘a’, ‘d’ Move left, right • SPACE BAR Move up • ‘c’ Move down  

### Custom mappings
Key mapping that do not follow the assignment guidelines but may be easier to use. 
• ↑, ↓, ←, → Look up, down, left, right  
• ‘w’, ‘s’, ‘a’, ‘d’ Move forward, back, left right  
• SPACE BAR Move up  
• ‘c’ Move down  