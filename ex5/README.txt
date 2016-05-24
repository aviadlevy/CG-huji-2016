aviadle
mosheperetz

EX5
==============================================
--------------  Description  -----------------
==============================================

We implemented Ellipsoid class that can generate the sphere class (if x=y=z), and also used this class to implement our
own AMAZING Sparoid (when x = y != z). we used this sparoid to create some funny scene. use "make scene4" to see this.
we almost didn't touch the given API, and tried to stick as close to it as possible.
in MyMeshObject we used the previous exercises.

We enjoyed very much to play with the scenes and try to draw some funny stuff. (without any additional value, so we didn't add them)
all in all, that was a great ex to finish the course with.

==============================================
----------------  Files  ---------------------
==============================================

# list of all the sources files
SRCS	=  	bimage.cpp      \
            ex5.cpp         \
            scene.cpp       \
            triangle.cpp    \
            camera.cpp      \
            object.cpp      \
            polygon.cpp     \
            MyMeshObject.cpp\
            lights.cpp      \
            sphere.cpp      \
            Ellipsoid.cpp   \
            Sparoid.cpp     \

# list of all the headers files
HDRS	=  	camera.h        \
            general.h       \
            lights.h        \
            MyMesh.h        \
            MyMeshObject.h  \
            MyVecs.h        \
            object.h        \
            polygon.h       \
            profiling_block_timer.h \
            ray.h           \
            scene.h         \
            Scene1.h        \
            Scene2.h        \
            Scene3.h        \
            Scene4.h        \
            SceneChooser.h  \
            sphere.h        \
            triangle.h      \
            Ellipsoid.h     \
            Sparoid.h       \
            bimage.h        \

ADDONS   = meshes/girl_face.obj \
          textures/checkerboard_lg.bmp \
          textures/mickey.bmp \
          textures/warning.bmp

===============================================
----------------   URLs   ---------------------
===============================================

http://homepages.paradise.net.nz/nickamy/simpleraytracer/simpleraytracer.htm
http://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing
