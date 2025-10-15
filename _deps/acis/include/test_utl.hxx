// $Id: test_utl.hxx,v 1.5 2001/03/15 21:11:36 gssl Exp $
/*******************************************************************/
/*    Copyright (c) 1989-2020 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/

#if !defined( SMOKE_TEST_UTIL_HXX )
#define SMOKE_TEST_UTIL_HXX

#include <stdarg.h>
#include <string.h>

#include "transf.hxx"
#include "body.hxx"
#include "ckoutcom.hxx"
#include "kernapi.hxx"
#include "ent_utl.hxx"
#include "vlists.hxx"


//-----------------------------------------------------------------------------------
// transforms
//-----------------------------------------------------------------------------------


SPAtransf rot_about_x(double angle);
SPAtransf rot_about_y(double angle);
SPAtransf rot_about_z(double angle);
SPAtransf move_along_x(double distance);
SPAtransf move_along_y(double distance);
SPAtransf move_along_z(double distance);

//-----------------------------------------------------------------------------------
// For spiral-shifting bodies
//-----------------------------------------------------------------------------------



// For displaying bodies in a spiral pattern
class spiral_display
{
private:
	int body_no;	    	// entity number, we start counting entities with 0
	double x;               // x-coordinate (y-coordinate, respectively) of SPAposition at which body
	double y;               // is going to be displayed as multiple of xshift (yshift,respectively).
	double  xshift;		    // distance between two displayed bodies
	double  yshift;		    // "
	double  zshift;         // spiral SPAposition  along z-axis
	double desired_diagonal;// bounding SPAbox to which we will scale the body before display
	double r;               // red
	double g;               // green
	double b;               // blue
	logical up;
public:
	spiral_display(double xdistance,double ydistance,double z,double desired_box_diagonal);
	int get_body_no();
	void set_body_no(int n);
	void change_the_colour();
	void display_entities(ENTITY* entity,logical change_colour = TRUE,double scaling_factor = 0.0);
	void display_entities(ENTITY_LIST& entities,logical change_colour = TRUE,double scaling_factor = 0.0);
	void change_xy();
};


//-----------------------------------------------------------------------------------
// For testing:
//-----------------------------------------------------------------------------------


class acis_test_unit
{
	// For spiral display:
	spiral_display spiral;

	// For printing out possible input:
	VOID_LIST names;
	VOID_LIST api_s;
	VOID_LIST functionality;
	logical a_test_matched;
	logical a_test_failed;

	// For printing out Test results:
	char *current_test;
	VOID_LIST test_results;

	// Methods:
public:
	acis_test_unit(double x_move=40.0, double y_move=40.0, double z_move=0.0, double desired_diagonal = 30.0);
	~acis_test_unit();
	void set_spiral_body_no(int n);
	int get_spiral_body_no();

	// Determines whether a test will be run.
	int runme(char* stringtest, char* test_name, int indicator, char* date,...);

	// Checks and displays the entity:
	void handle_entity(outcome const& result,
					   ENTITY* entity,
		               int vertex_no = -1,
					   logical change_colour = TRUE,
					   double scaling_factor=0);
	void handle_entities(outcome const& result,
		                 ENTITY_LIST&  entities,
		                 int* vertex_no_list,
					     int number_of_entities,
					     logical change_colour = TRUE,
					     double scaling_factor=0);


	// The following two signatures will be taken out.
	void handle_entity(ENTITY* entity,int vertex_no = -1,logical change_colour = TRUE,double scaling_factor=0); // checks and displays the entity
	void handle_entities(ENTITY_LIST&  entities,int* vertex_no_list,int number_of_entities,logical change_colour = TRUE,double scaling_factor=0);

	// Utility functions:
	void add_string(VOID_LIST &string_list,char* string);

	logical IsTestFail();

};


//-----------------------------------------------------------------------------------
// For manipulating strings:
//-----------------------------------------------------------------------------------
void print_void_list(VOID_LIST& string_list);


//-----------------------------------------------------------------------------------
// For manipulating the view:
//-----------------------------------------------------------------------------------
void update_view(SPAvector direction, SPAvector up);

#endif
