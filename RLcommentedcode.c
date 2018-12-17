// Created by Nick Livingston 2017
// Modified by Rena Lee, Xin Yue Hu, and David Schwitzgebel 2018

//*************************************** Function Declarations **************************************//

//Behavior Primitive Functions
int check_bluered();
int check_siren();                              // In this section of the code, we declared
void run_away();                                // the behavior functions which will be
                                                // defined further down in the code. The
int check_escape_conditions();                  // top three functions are our only original
void escape();                                  // functions; the rest were directly
                                                // appropriated from the original code. 
int check_seek_dark_conditions();
void seek_dark();
                                                // 'int' functions return integer values, 
int check_avoid_conditions();                   // which is why we used them for condition
void avoid();                                   // checking functions. The output from 'int'
                                                // functions were used in threshold-based 
void straight_cruise();                         // if statements in the actuating motor
void stop();                                    // functions, which are 'void' non-objects
								                // that do not return a value, they simply
								                // take sensory input and cause a motor
                                                // output. 
//Motor Control Function
void drive(float left, float right, float delay_seconds); 
                                                // The arguments for this motor function are
                                                // the left motor activation strength, the
                                                // right motor activation strength, and the
                                                // length of time the function is carried out,
                                                // in seconds, respectively. The drive 
                                                // function is called in all the other motor 
                                                // functions using different arguments to 
                                                // create specific driving behaviors. 

//************************************ Global Constant Declarations ************************************//
const int FRONT_BUMP = 0;
const int BACK_BUMP = 1;
const int LEFT_PHOTO = 2;                       // These first seven global constant declarations
const int RIGHT_PHOTO = 3;                      // assign sensory input objects (which will be 
const int LEFT_IR = 4;                          // used in condition checking functions) to the 
const int RIGHT_IR = 5;                         // anaglog ports (by number) on the KIPR controller. 
const int MIC_READ = 6;							// MIC_READ was the only new analog input we used.

const int LEFT_MOTOR = 0;						// These two constants assign the servo motors
const int RIGHT_MOTOR = 3;						// to their connection ports on the KIPR controller.
												// They are called in the drive function when
const float SERVO_MAX = 2047.0;					// actuating the movement of the motors. 
const float TOP_SPEED = 1023.0;	

float left_photo_value;							// These global constants were appropriated directly
float right_photo_value;						// from N Livingston's 'sensors.h' file and are
int photo_offset = 80;							// involved in the default behaviors of the robot:
float photo_coeff = 5.0;						// seek_dark, avoid, and escape. They use input
float photo_difference;							// from the photosensors and IR proximity sensors.
float photo_ratio;
float photo_threshold = 40.0;	
float photo_max = 1023.0;
int left_ir_value;
int right_ir_value;
int avoid_threshold = 300;

//Boolean Constants								// We assigned integer values to the Boolean 
const int TRUE = 1;								// constants in case they needed to be used in an 
const int FALSE = 0;							// integer form. The only instance of using an 
												// integer Boolean was in the  while loop of the 
												// main function. 
//**************************************** Function Definitions **************************************//
int main() 
{												// The main function initiates the connection
	int i;										// between the robot and camera, and prepares  
	enable_servos();							// the robot to activate the motors. These are from
  	camera_open(LOW_RES);						// the KIPR motor and sensor function libraries. 
												
	while(1){									// In an infinite while (true) loop, the function
    if(check_red()&&check_siren()){				// loops through a behavior hierarchy where 
      run_away();								// its top priority is looking for police-like
    }											// stimuli and running away from it. Otherwise,
		else if(check_escape_conditions()){		// the robot's default behavior assimilates
			escape();							// that of the robot ethology lab robots. This
		}										// leads the robot to travel a random and 
		else if(check_avoid_conditions()){		// unpredicted path in its environment, so once
			avoid();							// the intented target behavior (running away)
		}										// is activated, we can more easily observe it. 
		else{
			straight_cruise();
		}
	}	
	return 0;									// Returning 0 from the main function is necessary
}												// because the function is design to return an 
												// integer value. The function returning 0 is a 
												// way of indicating that the function worked 
												// properly, like an 'Exit Status' of the 
												// application.  
/******************************************************************************************************/		
int check_redblue(){							// **Checking for colors:
  camera_update();								// Use of the camera in our sensory inputs
  if((get_object_count(0)>0)					// involved employing some functions built-in in 
  &&(get_object_area(0,0)>20){					// the vision library (camera_object and get_object
  //&&(get_object_count(1)>0)					// functions), which were outlined in the KIPR
  //&&(get_object_area(1,0)>20){				// user manual. Channel 0 was configured on
	  //printf("red n blue!\n");				// the controller to detect red-colored objects, 
    return TRUE;								// and Channel 1 was configured to detect blue- 
  }												// colored objects. This configuration was saved		
  else{											// to the controller under Settings>Channels.		
	  	//printf("free!\n")						// The function's purpose was to return TRUE (1)		
		return FALSE;							// if it detected both red and blue objects with		 
	}    										// areas greater than 20 pixels in the camera's.		
}												// visual field. One of the more cumbersome		
												// obstacles in this project was having the 
												// camera recognize both colors at once. Due to 
												// lighting constraints and other issues involving 
												// the camera's functions (random and unwarranted
												// self-disconnection), we elected to have the
												// function check only for red objects in the 
												// visual field, so that it would more readily
												// recognize stimuli that it should run away from. 
												// Confirmatory print statements were removed from the
												// function, but were vital in our assessment of the
												// success of the function.  
/******************************************************************************************************/
int check_siren(){								// **Checking for sound:
	int sound_threshold = 45;					// A threshold for the analog input from the 
	int sound_value = analog_et(MIC_READ);		// microphone was set somewhat arbitrarily to 45. 
	if(sound_value >= sound_threshold){			// The microphone was best at detecting large and 
		//printf("siren\n");					// sudden changes in sound frequency, so we set it 
		return TRUE;							// to a value reasonably above its baseline reading, 
	}else{										// which was 27. This function works by returning  
		//printf("no sound\n");					// TRUE (1) if the reading from analog port 6, the
		return FALSE;							// microphone, is greater than our set threshold. 
	}											// If the threshold is not met, it returns FALSE (0)  
}												// and its motor function remains unaffected.
												// Again, confirmatory print statements were removed 
												// from the function, but were vital in our assessment 
												// of the success of the function.

												// **Combined stimuli conditions:
												// As you can see in line 76, the 'if' clause that 
												// activates the run_away function (below) is 
												// dependent upon simultaneous detection of the  
												// target color objects and sound value. So, the 
												// robot would not run away from sound stimuli or  
												// color stimuli alone; only when they were both  
												// detected at some point in time was the run_away 
												// function activated. 
/******************************************************************************************************/		
void run_away(){								// This function called the drive function, which is
  drive(0.5, 0.5, 0.5);							// defined starting in line 246. It causes the robot 
  msleep(8000);									// to drive backwards at a faster speed than normal.
}												// We found that the robot's motor directions were 
												// somehow configured to be backwards, so in this 
												// function the left and right motors are set to drive
												// at +0.5, whereas in the straight_cruise function 
												// (line 241), they are set to be -0.2. The msleep 
												// function causes the robot to perform its previous
												// behavior (driving backwards) for 8000 milliseconds, 
												// or 8 seconds. Once the robot completed this behavior, 
												// unless the stimuli were still present, it began
												// resuming its default behavior of random cruising. 
/******************************************************************************************************/
int check_escape_conditions(){					// The rest of the function definitions from here on
	int bump_threshold = 0;						// are directly appropriated from the base code 
	int bump_max = 400;							// written by N Livingston. The way these condition
	int front_bump_value = analog10(FRONT_BUMP);// checking functions and motor functions were
												// written largely influenced the design of our 
	if(front_bump_value >= bump_threshold 		// novel behavior functions. This function takes the
	&& front_bump_value <= bump_max){			// analog input from the front bumper, and if it
		return TRUE;							// falls within a specified range (between the
	}else{										// threshold and the maximum), it will return TRUE
		return FALSE;							// (1). The baseline reading for the bumper was
	}											// around 1000, and decreased with applied pressure. 
}
/******************************************************************************************************/
void escape(){									// If the escape_conditions function returned TRUE,					
	int bump_threshold = 250;					// in the main function it triggered the activation
	int bump_max = 400;							// of the escape behavior (lines 79-80). Notice that
	int front_bump_value = analog10(FRONT_BUMP);// there are two separate motor activations here.
												// There are two sets of ranges into which the bumper 
	if(front_bump_value < bump_threshold){		// sensor value can fall between. Depending on that 
		drive(0.0, 0.4, 0.50);					// value, the robot will make a turn to the left or 
	}											// to the right. This is done by sending motor 
	else if(front_bump_value >= bump_threshold  // activation to only the left or right servo motor 
	&& front_bump_value <= bump_max){			// (wheel), and keeping the other stationary. 
		drive(0.4, 0.0, 0.50);
	}
}
/******************************************************************************************************/
int check_seek_dark_conditions(){				// This function uses the analog value from the two
	int left_photo_raw;							// photosensors to create variables which keep track
	int right_photo_raw;						// of each raw value, a manipulated version of that
	float right_coeff;							// value in relation to the maxiumum input value, and
	float left_coeff;							// the different between the left and right values. 
	left_photo_raw = (analog_et(LEFT_PHOTO) - photo_offset);
	right_photo_raw = (analog_et(RIGHT_PHOTO));	// The photosensors are sensitive to 
												//
	left_coeff = (photo_coeff * (photo_max - (float)left_photo_raw)/photo_max);
	right_coeff = (photo_coeff * (photo_max - (float)right_photo_raw)/photo_max);
	
	left_photo_value  = left_photo_raw * left_coeff;
	right_photo_value = right_photo_raw * right_coeff;
	
	if(left_photo_value > photo_max){			// This part of the function sets the maxiumum value
		left_photo_value = photo_max;			// for the analog photo input. This helps for the
	}											// robot to keep track of which side of the environment,
	if(right_photo_value > photo_max){			// the left or the right, has a greater brightness than
		right_photo_value = photo_max;			// the other. If there is a significant difference
	}											// between the left and right adjusted photo values,
												// the function will return TRUE (1). 
	photo_difference  = left_photo_value - right_photo_value;

	if(abs(photo_difference) > photo_threshold){
		return TRUE;							// Returning TRUE (1) will activate the seek_dark
	}											// function, described below. This function was
	else{										// ultimately omitted fro the main function on the
		return FALSE;							// last day of testing due to conflicts in the testing
	}											// environment. If there was not sufficient lighting, 
}												// the camera could not detect the presence of the 
												// target colors, but if the obstacles were always well
												// lit in a darkened arena, there is no way the robot
												// would be in a position to see them in the first
												// place because it would be avoiding the light. 
/******************************************************************************************************/
void seek_dark(){								// Once activated, this function caused the robot to
	float left_servo;							// send different motor activations to each of the
	float right_servo;							// servo motors, based on the photosensor value it
												// processed in the above function. 
	if(fabs(photo_difference) > photo_threshold){
		left_servo  = photo_difference/photo_max;
		right_servo = -left_servo;				// It caused the robot to have stronger activation (to
	}											// rotate with greater speed) in the motor/wheel that
	drive(left_servo, right_servo, 0.25);		// corresponded to the photosensor that had a lower
}												// photo value. This caused the robot to turn toward 
												// the direction with less light. Once it turned,
												// the robot would resume its default behavior of 
												// driving straigt (straight_cruise). As mentioned
												// above, this part of the behavior hierarchy was 
												// completely omitted due to conflicts with the visual
												// input from the camera. 
/******************************************************************************************************/
int check_avoid_conditions(){					// This function worked by using the analog inputs from
	int left_ir_value;							// the two IR proximity sensors. The IR sensors work by
	int right_ir_value;							// detecting objects within 10 to 80 cm proximity. There
												// is a small emmitter and a detector on each IR sensor,
	left_ir_value = analog_et(LEFT_IR);			// and the whole unit keeps track of the time difference
	right_ir_value = analog_et(RIGHT_IR);		// between when a certain wavelength transittance is
												// emitted and when it is received. The time is takes 
	if(left_ir_value > avoid_threshold			// for the transittance signal to travel is indicative
	||right_ir_value > avoid_threshold){		// of the distance to the object. The IR sensors provided
		return TRUE;							// analog input to the KIPR controller. If either of the
	}else{										// IR sensors detected an object closer than the 
		return FALSE;							// threshold, this function returned TRUE (1) and the
	}											// robot would avoid the object.
}
/******************************************************************************************************/
void avoid(){									// Depending on the analog IR inputs from the sensors,
	int left_ir_value;							// this function caused the robot to turn to the right
	int right_ir_value;							// or to the left when confronted with nearby obstacles.
												// The avoid behavior is different from the escape and
	left_ir_value = analog_et(LEFT_IR);			// seek dark behaviors in that both motors were
	right_ir_value = analog_et(RIGHT_IR);		// activated at the same time, cause the robot to rotate
												// in place before driving away from an obstacle. 
	if(left_ir_value > avoid_threshold){
		drive(-0.25, 0.25, 0.1);
	}
	else if(right_ir_value > avoid_threshold){
		drive(0.25, -0.25, 0.1);
	}
}
/******************************************************************************************************/
void straight_cruise(){							// This was the default behavior of the robot. If there
	drive(-0.2, -0.2, 0.5);						// were no stimuli activating the other motor functions,
}												// the robot drove straight at a low speed. 
/******************************************************************************************************/
void stop(){									// The stop function was not used in the original code, 
	drive(0.0, 0.0, 0.25);						// but we implemented it while testing the behavior of
}												// the robot. One of our greatest obstacles to creating
												// the target behavior was synthesizing all of the
												// different components of the behavior together. To
												// more carefully observe the robot's behavior and 
												// demarcate the different motor functions we tried
												// to establish, we had the robot stop in between
												// functions. This allowed us to assess which functions
												// needed to be adjusted. 
/******************************************************************************************************/
void drive(float left, float right, float delay_seconds){	
	float left_speed;							// This is the function that is called in all other
	float right_speed;							// motor functions, which activates the two positional
												// servo motors that rotate with the wheels of the robot.
	delay_seconds *= 1000;						// When called in other functions, we had to supply
												// three arguments: left, right, and delay_seconds. The
	right_speed = TOP_SPEED - (right*TOP_SPEED);// left and right arguments set the speed of rotation
	left_speed = TOP_SPEED + (left*TOP_SPEED);	// for the corresponding motors as a function of the
												// robot's maximum speed. The delay_seconds keeps the
	set_servo_position(LEFT_MOTOR, left_speed);	// drive function running for a set amount of time.  
	set_servo_position(RIGHT_MOTOR, right_speed);
	msleep(delay_seconds);
}
/*************************************** OMITTED FUNCTIONS ********************************************/
// The following portions of code were experimental and did not end up being used in the final
// demonstration of the robot's behavior. While individual functions worked properly when tested on 
// their own, our earlier versions of the main function arranged these functions in such a way as to
// not produce any behavior. Two of the earlier versions of the main function are also displayed below.
/******************************************************************************************************/
int check_run_conditions(){						// We initially had a another function that grouped the
	if(check_siren()&&check_redblue()){			// two stimulus checking functions together. We found
    	//printf("run\n");						// that this step was uneccessary and that we could
        return TRUE;							// call them separately in the same if clause in the
	}											// main function rather than getting deeper into nested 
	else{										// functions.
	  	//printf("fine\n");
    	return FALSE;
	}
}

/******************************************************************************************************/
int check_paranoia_conditions(){				// This was a function designed to make the robot check
	if(check_siren()||check_redblue()){			// that either the sound of visual stimuli were present. 
		//printf("paranoia\n");					// It was going to be used to trigger the robot to 
        return TRUE;							// stop and rotate in place, so that it would be able to
	}											// spot the police stimuli if they were not in its 
	else{										// direct line of sight or hearing. 
	  	//printf(“safe\n”);
	   return FALSE;
	}
}
/******************************************************************************************************/
void paranoia(){								// The above function would have triggered this 
	drive(0.05, -0.05, 5);						// function, which sends equal and opposite activations
}												// to the two servos, causing it to rotate in place for
												// 5 seconds. 
/******************************************************************************************************/
 void hide(){									// This function was going to be used to have the robot
	msleep(10000);								// stay in place once it had found a dark corner. It 
	paranoia();									// would perform stimulus checking (paranoia) if it 
}												// was picking up on either the visual or auditory
												// stimuli. 
/******************************************************************************************************/
void sketchstride(){							// This function served as a version of the default
	//enable_servos();							// behavior for the finite state machine structured main
	if(check_avoid_conditions()){				// function. It was ultimately commented out from that
		avoid();								// version of the main function because it still caused
	}											// an issue with nested behavior hierarchies. 
	else if(check_escape_conditions()){
		escape();								// After multiple attempts and manipulations to this
	}											// function and its place in the finite state machine
	else if(check_seek_dark_conditions()){		// structured main functions, we abandoned the idea
		seek_dark();							// completely and returned to a simplified version of
	}											// the original behavior hierarchy. You may notice that
	else{										// this function in itself is very similar to the 
		straight_cruise();						// current main function. It works in the same way, 
	}											// enacting certain behaviors once the sensory 
}												// conditions have been met. 
/******************************************************************************************************/
void le_default(){								// This function is essentially the same as the one 
	//enable_servos();							// above, but we deleted the seek_dark component of the
	if(check_avoid_conditions()){				// behavior after discovering the conflict that lighting
		avoid();								// conditions had with the camera's ability to detect
	}											// color. We placed it at the bottom of the original 
	else if(check_escape_conditions()){			// behavior hierarchy which included running and 
		escape();								// paranoia behaviors, but upon consultation we found
	}											// that the nested hierarchy would be difficult for
	else{straight_cruise();						// the program to implement. 
	}
}
/******************************************************************************************************/
int main_hierarchy1(){							// We are still unsure as to why this function did not
	int i;										// work other than the fact that there was a behavior
	enable_servos();							// hierarchy (le_default) within the behavior hierarchy. 
	camera_open(LOW_RES);						// One attempt to solve the issue was write out the 
	display_clear();							// entire le_default function into the main hierarchy, 
												// but that was also unsuccessful. One issue may have 
	while(TRUE){								// been with the combined stimuli checking function
		if(check_run_conditions){				// (check_run_conditions). We found that at times the
            run();								// camera was not correctly functioning and picking
		}										// up on the target color objects. Still, even with
		else if(check_paranoia_conditions()){	// the camera malfunctioning, the robot should have 
			paranoia(); 						// been able to carry out the other functions, but with
		}										// this version of the main function in the KIPR program,
		else{									// the robot stood stationary and made small, slow 
			le_default();						// rotations at seemingly random times, which we assumed
		}										// were mechanical glitches because the computational
	}											// load on the controller was too heavy. 
	return 0;
	camera_close();
}
/******************************************************************************************************/
int main_finitestate(){							// After failing to produce a functioning behavior
	int i;										// hierarchy, we attempted to construct a finite state
	enable_servos();							// machine which was also ultimately unsuccessful.
	camera_open(LOW_RES);		
	//display_clear();
	
	while(TRUE){
		if(check_run_conditions){				// We incorporated the structure of some of the other
            int left_photo_value;				// condition checking functions within the main while
	        int right_photo_value;				// loop in order to declare the sensor value objects.

		    left_photo_value = analog_et(LEFT_PHOTO);
		    right_photo_value = analog_et(RIGHT_PHOTO);
			run();								// This was the 'running state' of the robot. If both
												// visual and auditory threat stimuli were detected, it
												// would automatically be running. 
	        while(left_photo_value >= 50 && right_photo_value >= 50){
		        //sketchstride();				// The sketchstride function was the default behavior 
		        if(check_avoid_conditions()){	// that had no dependence on environmental conditions. 
					//printf(“avoid\n”);		// We ended up removing it because we realized the same
					avoid();					// issue with nested behavior hierarchies had shown up. 
      			}							
				else if(check_escape_conditions()){
					//printf(“escape\n”);		// This while loop theoretically was supposed to be
					escape();					// once the robot had already run away and was in a 
                }								// dark region of the arena. 
				else if(check_seek_dark_conditions()){
					//printf(“seek dark\n”);
					seek_dark();
      			}
				else{
					printf(“cruise\n”);
					straight_cruise();
                }
			}
			//printf(“hide”);
		 	hide();								// We had the robot hiding once it was in an escaped
 	        }									// state from the poolice stimuli, and checking its
		else if(check_paranoia_conditions()){	// surroundings to be sure that it was in a safe place. 
			paranoia(); 
		}
		else{
			if(check_avoid_conditions()){		// This version of the function has been manipulated 											// from the original but is meant to show an amalgamation
				avoid();}						// of the different things we tried. At no point were there
			else if(check_escape_conditions()){	// functions called multiple times in different loops.
				escape();						// This section was the default behavior section, where if
			}									// the robot was neither running nor hiding, it was taking
			else{								// a random walk through the arena. 
				straight_cruise();
			}
		}
	return 0;
	camera_close();
}

