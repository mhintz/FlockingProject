#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class FlockingProjectApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void FlockingProjectApp::setup()
{
}

void FlockingProjectApp::mouseDown( MouseEvent event )
{
}

void FlockingProjectApp::update()
{
}

void FlockingProjectApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( FlockingProjectApp, RendererGl )
