

//
//  Scene4.h
//  cg-projects
//
//

#ifndef cg_projects_Scene5_h
#define cg_projects_Scene5_h
#include "Sparoid.h"

struct Scene4 : public Scene
{

    Scene4()
            : Scene()
    {
        defineGeometry();
        defineLights();
    }

    void defineLights()
    {
        Scene & scene = *this;
        Point3d pos(10,100,10);
        Color3d color(1,1,1);
        PointLight  * p = new PointLight(pos,color, 10);
        scene.add_light(p);

        Point3d pos1(10,20,30);
        Color3d color1(1,1,1);
        PointLight  * p1 = new PointLight(pos1,color1);
        scene.add_light(p1);


    }

    void defineGeometry()
    {
        Scene & scene = *this;
#if !WITHOUT_TEXTURES
        BImage * mickey = new BImage("textures/mickey.bmp");
#endif
        /* define some colors */
        Color3d white(1.0);
        Color3d black(0.0);
        Color3d red(1,0,0.0);
        Color3d green(0,1.0,0.0);
        Color3d blue(0,0,1.0);
        Color3d iron( 0.30, 0.30, 0.30);

        scene.backgroundColor() = (iron +  white) * 0.5;

        Point3d center3(0,0,10);
        double radius = 3;
        Vector3d scale(1,2,1);
        Ellipsoid * sp3 = new Sparoid(center3,radius, 1, 2);
        sp3->diffuse() = iron;
        sp3->reflection() = white*0.5;
        sp3->specular() = white * 0.5;
        sp3->shining() = 40;
        scene.add_object(sp3);

        Point3d center(0,8,10);
        double radius1 = 2.5;
        Sphere* head = new Sphere(center,radius1);
        head->diffuse() = iron;
        head->reflection() = white*0.3;
        head->specular() = white;
        head->shining() = 60;
        scene.add_object(head);


        Point3d center1(3,10,10);
        double radius2 = 1;
        Sphere* ear1 = new Sphere(center1,radius2);
        ear1->diffuse() = black;
        ear1->reflection() = black*0.5;
        ear1->specular() = white;
        ear1->shining() = 60;
        scene.add_object(ear1);

        Point3d center2(-2.9,10,10);
        double radius3 = 1;
        Sphere* ear2 = new Sphere(center2,radius3);
        ear2->diffuse() = black;
        ear2->reflection() = black*0.5;
        ear2->specular() = white;
        ear2->shining() = 60;
        scene.add_object(ear2);

        Point3d center4(-0.3,7,15);
        double radius4 = 0.3;
        Sphere* nose = new Sphere(center4,radius4);
        nose->diffuse() = red;
        nose->reflection() = red*0.5;
        nose->specular() = white;
        nose->shining() = 60;
        scene.add_object(nose);

        //create a plane
        vector<Point3d> plane(4);
        vector<Point2d> plane_uv(4);
        double x = 100;
        double z = -4;
        plane[0] = Point3d(-x,z,-x);
        plane[1] = Point3d(-x,z,x);
        plane[2] = Point3d(x,z,x);
        plane[3] = Point3d(x,z,-x);
        plane_uv[0] = Point2d(0,0);
        plane_uv[1] = Point2d(0,1);
        plane_uv[2] = Point2d(1,1);
        plane_uv[3] = Point2d(1,0);
        Polygon * poly = new Polygon(plane,plane_uv);
        poly->diffuse() = iron;
        poly->reflection() = white * 0.4;
#if !WITHOUT_TEXTURES
        poly->set_texture_map(mickey);
#endif
        scene.add_object(poly);
    }

    virtual void setDefaultCamera(Camera& camera) const
    {
        Point3d pos(-1,6,25);
        double fov_h = 50 / 180.0 * M_PI ;
        Point3d coi(0,0,-0);
        Vector3d up(0,1,0) ;
        //		Point3d pos(0,8,35);
        //		double fov_h = 30 / 180.0 * M_PI;
        //		Point3d coi(0,0,-0);
        //		Vector3d up(0,1,0) ;
        camera = Camera(pos,coi,up,fov_h);
    }

    virtual ~Scene4() {

    }

};


#endif