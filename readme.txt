COMP 371 WINTER 2016 - TERM PROJECT - BLOCK WORLD 3D

I . The team
--------------

David Tsatsoulis - 25410029
Simon Jacques - 27046677
Tri-Luong Steven Dien - 27415281
Pierre-Olivier Jourdenais - 26987540

II. Block World 3D
------------------

Block World 3D is a recreation of minecraft, with fully procedurally generated terrain, randomly generated trees, a rotating light source, 
multiple camera modes. 

Here are some of the features of Block World 3D

III. Features
---------------

1. Collision system

The collision system is based around checking the height of the surrounding blocks. 
When the user encounters a block that is at a higher height than the block he is currently on,
the camera stops moving. The user must then either go elsewhere, or jump on the ledge to continue.

2. Camera system

Block World 3D has multiple camera modes. First is the classic FPS camera, with the controller walking around
the world. This is the default camera mode. The second camera mode is a third person camera, used to look at your immediate surroundings.
The third camera is a fly by camera, used to fly around the world.

3. Light system

There is a rotating light source, simulating a sun. The intensity of the light varies based on a timer,
to simulate the day and night cycle.

4. Character model

We have a fully modeled character, who is riding around on a very nice Segway! The character is fully modeled
in the style of Minecraft.

5. World generation

Block World 3D's terrain is fully procedurally generated. We used Ken Perlin's classic 'Perlin Noise' algorithm
to randomly generate a terrain based on heightmaps. We also have the ability to create our own 
maps, however these maps tend to vary wildly on quality, and aren't always playable. Each block
that finds itself on the surface of the world is then textured with grass, and as a bonus, blocks also have a bit of grass
on their sides, just like Minecraft!

6. Randomized trees

Our trees are also randomized, based on a set selection of shapes and textures, for both the trunk and the leaves. The trees are then randomly placed
in the gameworld.

7. Music

We have a soothing song playing in the background, thus your stay in Block World 3D is sure to be relaxing. We also have footsteps playing
when the user is in camera mode 1 (normal FPS camera). Enjoy!

IV. How to play
-----------------

CONTROL

You control the character with the WASD keys.

W: Walk forward.
A: Strafe left.
S: Walk backwards.
D: Strafe left.
LEFT SHIFT: Used to toggle run mode. Hold to run, release to walk.
SPACE: Used to jump. Required to climb on higher terrain.
MOUSE: Used to look around. 

CAMERA

1: Default camera mode, FPS style camera. 
2: Third person, float in air style camera.
3: Fly by camera, used to fly around the world.

OTHER KEYS

ESCAPE: Exits the game.



