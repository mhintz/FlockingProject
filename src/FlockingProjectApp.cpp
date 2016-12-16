#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

vec2 limit(vec2 v, float l) {
	float lim2 = l * l;
	auto len2 = length2(v);
	if (len2 > lim2) {
		return normalize(v) * l;
	} else {
		return v;
	}
}

class FlockingProjectApp : public App {
  public:
	static void prepSettings(Settings * settings);

	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;

	float mBirdSize = 2.0f;
	float mMaxSpeed = 2.0f;
	float mMaxForce = 0.03f;

	int mWidth;
	int mHeight;

	vector<vec2> mPositions;
	vector<vec2> mVelocities;
	vector<vec2> mAccelerations;

	gl::VboRef mPositionsVbo;
	gl::VboMeshRef mPointMesh;
};

int const numBirds = 300;

void FlockingProjectApp::prepSettings(Settings * settings) {
	settings->setFullScreen();
}

void FlockingProjectApp::setup()
{
	mWidth = getWindowWidth();
	mHeight = getWindowHeight();

	for (int i = 0; i < numBirds; i++) {
		mPositions.push_back(randVec2() * randFloat() * vec2(getWindowSize() / 2) + vec2(getWindowSize() / 2));
		mVelocities.push_back(randVec2());
		mAccelerations.push_back(vec2(0));
	}

	mPositionsVbo = gl::Vbo::create(GL_ARRAY_BUFFER, mPositions, GL_STREAM_DRAW);
	auto posBufferLayout = geom::BufferLayout({ geom::AttribInfo(geom::POSITION, 2, 0, 0) });
	mPointMesh = gl::VboMesh::create(mPositions.size(), GL_POINTS, { { posBufferLayout, mPositionsVbo } });
}

void FlockingProjectApp::mouseDown( MouseEvent event )
{
}

void FlockingProjectApp::update()
{
	for (int i = 0, l = mPositions.size(); i < l; i++) {
		vec2 & pos = mPositions[i];
		vec2 & vel = mVelocities[i];
		vec2 & acc = mAccelerations[i];

		static float separationNeighborDist = 25.0f;
		static float alignNeighborDist = 50.0f;
		static float cohesionNeighborDist = 50.0f;
		// separation from neighbors
		vec2 sepSteer = vec2(0);
		int separationNeighbors = 0;
		// average alignment of neighbors
		vec2 alignSteer = vec2(0);
		int alignmentNeighbors = 0;
		// average position of neighbors
		vec2 cohesionPosition = vec2(0);
		int cohesionNeighbors = 0;

		for (int idx = 0, l = mPositions.size(); idx < l; idx++) {
			vec2 & otherPos = mPositions[idx];
			vec2 & otherVel = mVelocities[idx];
			float otherDist = distance(pos, otherPos);

			// the boids are not the same
			if (otherDist > 0) {
				// separation
				if (otherDist < separationNeighborDist) {
					vec2 fromOther = normalize(pos - otherPos) / otherDist;
					sepSteer += fromOther;
					separationNeighbors += 1;
				}

				// alignment
				if (otherDist < alignNeighborDist) {
					alignSteer += otherVel;
					alignmentNeighbors += 1;
				}

				// cohesion
				if (otherDist < cohesionNeighborDist) {
					cohesionPosition += otherPos;
					cohesionNeighbors += 1;
				}
			}
		}

		if (separationNeighbors > 0) {
			sepSteer /= separationNeighbors;
			// It is possible for the separation vectors to cancel each other out, for example
			// if there are two neighbors directly opposite each other relative to the bird
			if (length2(sepSteer) > 0) {
				sepSteer = (normalize(sepSteer) * mMaxSpeed) - vel;
				sepSteer = limit(sepSteer, mMaxForce);
			}
		}

		if (alignmentNeighbors > 0) {
			alignSteer /= alignmentNeighbors;
			alignSteer = (normalize(alignSteer) * mMaxSpeed) - vel;
			alignSteer = limit(alignSteer, mMaxForce);
		}

		vec2 cohesionSteer(0);
		if (cohesionNeighbors > 0) {
			cohesionPosition /= cohesionNeighbors;
			cohesionSteer = (normalize(cohesionPosition - pos) * mMaxSpeed) - vel;
			cohesionSteer = limit(cohesionPosition, mMaxForce);
		}

		sepSteer *= 1.5;
		alignSteer *= 1.0;
		cohesionSteer *= 1.0;

		acc += sepSteer;
		acc += alignSteer;
		acc += cohesionSteer;

		vel += acc;
		vel = limit(vel, mMaxSpeed);
		pos += vel;
		acc = vec2(0); // reset each cycle

		// Apply bounds
		if (pos.x < -mBirdSize) { pos.x = mWidth + mBirdSize; }
		if (pos.x > mWidth) { pos.x = -mBirdSize; }
		if (pos.y < -mBirdSize) { pos.y = mHeight + mBirdSize; }
		if (pos.y > mHeight) { pos.y = -mBirdSize; }
	}

	mPositionsVbo->bufferSubData(0, mPositions.size() * sizeof(vec2), mPositions.data());
}

void FlockingProjectApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) );

	gl::color(Color(1, 1, 1));

	gl::draw(mPointMesh);

	gl::drawString(std::to_string(getAverageFps()), vec2(10.0f, 20.0f), ColorA(1.0f, 1.0f, 1.0f, 1.0f));
}

CINDER_APP( FlockingProjectApp, RendererGl, &FlockingProjectApp::prepSettings )
