Uzair Akram LAB5

Compile using
gcc -o routing routing.c

Run using:
./routing

Run on router.txt
Run it first with the file provided

Example Run
Graph:
  u v w x y z
u 0 2 5 1 0 0
v 2 0 3 2 0 0
w 5 3 0 3 1 5
x 1 2 3 0 1 0
y 0 0 1 1 0 2
z 0 0 5 0 2 0

Dijkstra's Algorithm Table:
    u    v    w    x    y    z
u    0,u    2,u    5,u    1,u    ∞,u    ∞,u
ux    0,u    2,u    4,x    1,u    2,x    ∞,v
uxv    0,u    2,u    4,x    1,u    2,x    ∞,w
uxvy    0,u    2,u    3,y    1,u    2,x    4,y
uxvyw    0,u    2,u    3,y    1,u    2,x    4,y

MST:
  u v w x y z
u 0 2 0 1 0 0
v 2 0 0 0 0 0
w 0 0 0 0 1 0
x 1 0 0 0 1 0
y 0 0 1 1 0 2
z 0 0 0 0 2 0


Dijkstra's Algorithm Table:
    u    v    w    x    y    z
v    2,v    0,v    3,v    2,v    ∞,v    ∞,v
vv    2,v    0,v    3,v    2,v    ∞,v    ∞,v
vvu    2,v    0,v    3,v    2,v    ∞,w    ∞,w
vvux    2,v    0,v    3,v    2,v    3,x    ∞,x
vvuxw    2,v    0,v    3,v    2,v    3,x    8,w

MST:
  u v w x y z
u 0 2 0 0 0 0
v 2 0 3 2 0 0
w 0 3 0 0 0 5
x 0 2 0 0 1 0
y 0 0 0 1 0 0
z 0 0 5 0 0 0


Dijkstra's Algorithm Table:
    u    v    w    x    y    z
w    5,w    3,w    0,w    3,w    1,w    5,w
ww    5,w    3,w    0,w    3,w    1,w    5,w
wwy    1,y    1,y    0,w    2,y    1,w    3,y
wwyu    1,y    1,y    0,w    2,y    1,w    1,u
wwyuv    1,y    1,y    0,w    2,y    1,w    1,u

MST:
  u v w x y z
u 0 0 0 0 0 0
v 0 0 0 0 0 0
w 0 0 0 0 1 0
x 0 0 0 0 1 0
y 0 0 1 1 0 0
z 0 0 0 0 0 0


Dijkstra's Algorithm Table:
    u    v    w    x    y    z
x    1,x    2,x    3,x    0,x    1,x    ∞,x
xx    1,x    2,x    3,x    0,x    1,x    ∞,v
xxu    1,x    2,x    3,x    0,x    1,x    ∞,w
xxuy    1,x    1,y    2,y    0,x    1,x    3,y
xxuyv    1,x    1,y    2,y    0,x    1,x    1,v

MST:
  u v w x y z
u 0 0 0 1 0 0
v 0 0 0 0 0 0
w 0 0 0 0 1 0
x 1 0 0 0 1 0
y 0 0 1 1 0 0
z 0 0 0 0 0 0


Dijkstra's Algorithm Table:
    u    v    w    x    y    z
y    ∞,y    ∞,y    1,y    1,y    0,y    2,y
yy    ∞,v    ∞,v    1,y    1,y    0,y    2,y
yyw    6,w    4,w    1,y    1,y    0,y    2,y
yywx    2,x    3,x    1,y    1,y    0,y    1,x
yywxz    1,z    1,z    1,y    1,y    0,y    1,x

MST:
  u v w x y z
u 0 0 0 0 0 0
v 0 0 0 0 0 0
w 0 0 0 0 1 0
x 0 0 0 0 1 0
y 0 0 1 1 0 0
z 0 0 0 0 0 0


Dijkstra's Algorithm Table:
    u    v    w    x    y    z
z    ∞,z    ∞,z    5,z    ∞,z    2,z    0,z
zz    ∞,v    ∞,v    5,z    ∞,v    2,z    0,z
zzy    ∞,w    ∞,w    3,y    3,y    2,z    0,z
zzyw    8,w    6,w    3,y    3,y    2,z    0,z
zzywx    4,x    5,x    3,y    3,y    2,z    0,z

MST:
  u v w x y z
u 0 0 0 1 0 0
v 0 0 0 2 0 0
w 0 0 0 0 1 0
x 1 2 0 0 1 0
y 0 0 1 1 0 2
z 0 0 0 0 2 0

Link State Forwarding Table:

v (u, v)
w (u, x)
x (u, x)
y (u, x)
z (u, x)
--------
u (v, u)
w (v, w)
x (v, x)
y (v, x)
z (v, x)
--------
u (w, y)
v (w, v)
x (w, y)
y (w, y)
z (w, y)
--------
u (x, u)
v (x, v)
w (x, y)
y (x, y)
z (x, y)
--------
u (y, x)
v (y, x)
w (y, w)
x (y, x)
z (y, z)
--------
u (z, y)
v (z, y)
w (z, y)
x (z, y)
y (z, y)
--------
