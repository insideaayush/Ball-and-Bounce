******************COP-290 (DESIGN PRACTICES)**********************
***Authors:Aayush Gautam , Akash Tanwar                        ***
***DESCRIPTION:Screensaver with advance functionality          ***
***Date:25-01-2015                                             ***
***Time:22:37                                                  ***
******************************************************************


******Instructions************************************************

requirements: Linux with openGl and other libraries pre-installed
	installtion (openGl):
		1. Linux
			a) RedHat Linux
				yum install freeglut-devel

			b) Debian (e.g. Ubuntu)
				apt-get install freeglut3-dev
		2. OS/X
			already installed. enjoy!
	installation (LaTex):
		1. Ubuntu
			sudo apt install texlive-latex-base
	

basic:
    Type "make execute num_threads=n", where n is number 
    of threads i.e. balls
    
advance:
    makefile instruction:
        make: comiles all of the source file.(Note: it does not
              compile latex files i.e design document)
        make compile: same as above.
        make execute num_thrad=n: runs executable file with 
              n balls.
        make execute:run executable file.
        make DEBUG: Runs the program in cout statements-debug mode
        make doc: creates pdf of design document.
        make clean: cleans all temporary and executable files
            including PDFs.
    Keyboard Instructions
        key x,X: Rotates screen on x axis.
        key y,Y: Rotates screen on y axis.
        key z,Z: Rotates screen on z axis.
        key j,J: Navigation across x axis.
        key k,K: Navigation across y axis.
        key l,L: Navigation across z axis(ZOOM IN ,ZOOM OUT)
        key b: Rotates screen by 90 deg about z axis.
        key n: Rotates screen by 90 deg about z axis.
        key m: Rotates screen by 90 deg about z axis.
        key o,O: Resets the orientation.
        key s,S: pauses the scene.
        key p,P: Starts the motion.
        key esc: Exits the program
        key UP_ARROW: Rotates upwards.
        key DOWN_ARROW: Rotates Downwards.
        key LEFT_ARROW: Rotates Leftwards.
        key RIGHT_ARROW: Rotates Rightwards.
        key PAGE_UP: Rotates along z.
        key PAGE_DOWN:Rotates along z(opp).
    Mouse Instructions
        LEFT_CLICK: Pauses or starts the sce accordingly.
        RIGHT_CLICK: POP-UP menu to change the speed of the balls
