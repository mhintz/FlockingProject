.PHONY: build run

all: build run

build:
	xcodebuild -configuration Debug -project xcode/FlockingProject.xcodeproj/

run:
	./xcode/build/Debug/FlockingProject.app/Contents/MacOS/FlockingProject
