# Mesh Subdivision Extra Credit

Mesh subdivision is the process of increasing the resolution of a mesh by adding new vertices and faces, while preserving the overall shape of the original mesh. Generally, mesh subdivision is combined with smoothing in order to model a smooth, high resolution object with relatively few vertices. In this extra credit, we will combine a simple subdivision scheme with implicit fairing. Since you have already implemented implicit fairing, all you need to do is implement mesh subdivision and apply it to an appropriate dataset. We have provided the spock head used in the original implicit fairing paper so you can see the results of our mesh subdivision algorithm.

We will use a very simple subdivision scheme which splits each triangular face into 4 smaller triangles of equal size. The process is as follows:

1. Iterate over all the triangles in the mesh:
   1. Call the triangle's vertices $v1$, $v2$, and $v3$.
   2. Create new vertices at the midpoints of each edge of the triangle,(v1, v2)$, $(v2, v3)$, and $(v3, v1)$.
      - Call these vertices $m1$, $m2$, and $m3$, respectively.
   3. Create four new faces to replace the original triangle.
      1. Create a triangle from $v1$, $m1$, and $m3$
      2. Create a triangle from $v2$, $m2$, and $m1$
      3. Create a triangle from $v3$, $m3$, and $m2$
      4. Create a triangle from $m1$, $m2$, and $m3$
2. Calculate normals for the mesh as you did after smoothing.

Make sure that when you create the faces, the vertices are provided in the correct order so that the normals will be facing the correct direction.

You can either:

- Add an extra (optional) command line parameter that allows you to specify the level of subdivision to apply before smoothing the mesh. This parameter should be a positive integer corresponding to the number of times the above operation is repeated.
- Add a key that, when pressed, will apply a subdivision to the mesh.

Be aware that the number of faces increases by a factor of 4 after each subdivision, so your code should be as efficient as possible.

To test the subdivision, run your smoothing/subdivision code with `scene_spock.txt` with one iteration of subdivision. It should resemble the smoothed spock in the lecture notes.
