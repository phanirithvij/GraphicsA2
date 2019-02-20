# A simple 3D game made using OPENGL in C++

## To run

```shell

mkdir build/

cp -r src/assets/ build/
cd build/

mv assets/blend/* assets/
mv assets/obj/* assets/
mv assets/dds/* assets/

cmake ..
make

./graphics_asgn2

```

## Controls

`A`, `D` for tilting the aeroplane.

`W` to accelerate the aeroplane.

`H` to pause the plane.

`Q`, `E` to rotate the plane left and right.

`L` to show wireframe rendering.
`O` to show complete rendering.
`P` to show only points.

`UP` and `DOWN` to move it up and down.

`MOUSE RIGHT CLICK` and `MOUSE LEFT CLICK` for missiles and bombs.

`1`, `2`, `3`, `4`, `5`, `6` to change camera positions.

`1` for tower view.

`2` for follow cam.

`MOUSE_MIDDLE_BUTTON` and `3` and `scroll wheel` for Helicam view.

`4` for top view.

`5` for pilot view.

`6` for best view possible. Gamer view.

## Bonus

1. A compass to indicate direction (top-right corner).

2. Loop in a Loop when `V` is pressed.

3. Barrel roll when `B` is pressed.

<!-- 2.  -->