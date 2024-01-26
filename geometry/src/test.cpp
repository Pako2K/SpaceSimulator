#include <iostream>
#include <array>

#include <geometry/basic_types.h>
#include <geometry/point.h>
#include <geometry/unit_vector.h>
#include <geometry/cartesian_cs.h>
#include <geometry/shapes2D/ellipse.h>
#include <geometry/shapes2D/circle.h>
#include <geometry/shapes2D/rectangle.h>
#include <geometry/shapes3D/cone.h>
#include <geometry/shapes3D/cylinder.h>
#include <geometry/shapes3D/cuboid.h>
#include <geometry/shapes3D/ellipsoid.h>
#include <geometry/shapes3D/sphere.h>
#include <geometry/shapes2D/reg_polygon.h>
#include <geometry/projections.h>

using namespace geometry;


int main() {
  {
    std::cout << "********************** TYPES TESTING **********************************" << std::endl;
    std::cout << "***********************************************************************" << std::endl;
        
    Vec3<double> vv(1.1, 2.0, 4);
    Mat3<int> mm;
    Vec3<float> v1;
        
    Angle a{ 4.55 };
    std::cout << "angle: " << a << std::endl;
    std::cout << "angle: " << a() << std::endl;
    std::cout << "angle: " << double(a) << std::endl;
    Angle a1{ 30.55 };
    std::cout << "angle 1: " << a1.reduced() << std::endl;
    std::cout << "angle 1: " << a1.degrees() << std::endl;
    std::cout << "angle 1: " << radians(a1.degrees()) << std::endl;
    std::cout << "angle 1: " << a1.reduce() << std::endl;
    std::cout << "angle 1: " << a1.degrees() << std::endl;

    std::cout << "angles + " << a + a1 << std::endl;
    std::cout << "angles - " << a - a1 << std::endl;
    std::cout << "angles == " << (a == a1) << std::endl;
    std::cout << "angles != " << (a != a1) << std::endl;
    std::cout << "angles < " << (a < a1) << std::endl;
    std::cout << "angles > " << (a > a1) << std::endl;
    std::cout << "angles <= " << (a <= a1) << std::endl;
    std::cout << "angles >= " << (a >= a1) << std::endl;

    Angle a2{ a1 };
    Angle<float> a3;
    a3 = float(a2);

    //Angle int_a{ 7 };  // does not compile!!

    Length<float> ss;
    Length ss2{ 3.5f };
    //Size ss3{ 1 };
    //Size ss4{ -1 };
    Length ss3{ ss2 + 2.0f };
    std::cout << ss3 << std::endl;

    ss2 = 7.5;
    std::cout << ss2 << std::endl;

  }

  {
    std::cout << "********************** UNIT VECTOR TESTING ****************************" << std::endl;
    std::cout << "***********************************************************************" << std::endl;
    
    UnitVec<float, 4> v{ 1,3,0,0 };
    std::cout << v << std::endl;

    UnitVec<float, 2> v2(3, 5);
    std::cout << v2 << std::endl;
    std::cout << v2.y() << std::endl;
    //std::cout << v2.z() << std::endl;

    UnitVec<float, 3> v3(1, 0, 1);
    std::cout << v3 << std::endl;
    std::cout << v3[2] << std::endl;

    UnitVec<float, 3> v33(0, 0, 10);
    Vec3<float> v4{ Vec3<float>(0, 0, 1.2f).cross(v3.vec()) };
    std::cout << v4 << std::endl;
    std::cout << v3.cross(v33) << std::endl;
    Vec3<float> vvv(0, 1, 1.2f);
    std::cout << UnitVec<float, 3>(v33.cross(vvv)) << std::endl;

    std::cout << v4.dot(v33.vec()) << std::endl;
    std::cout << v3.dot(Vec3<float>(0, 0, 1.2f)) << std::endl; 
    std::cout << v4 + v33.vec() << std::endl;
    std::cout << v3 + Vec3<float>(0, 0, 1.2f) << std::endl;
    std::cout << v33 + v3 << std::endl;
    std::cout << v4 - v33.vec() << std::endl;
    std::cout << v3 - Vec3<float>(0, 0, 1.2f) << std::endl;
    std::cout << v33 - v3 << std::endl;
      

    UnitVec3<double> ux{ Vec3<double>::UnitX() };
    std::cout << ux << std::endl;

    std::cout << (v33 == v33) << std::endl;
  }
  
  {
    std::cout << "********************** POINT TESTING **********************************" << std::endl;
    std::cout << "***********************************************************************" << std::endl;
    //Point<2, std::string> pp;
    Point<int, 4> p;
    Point<float, 2> p2(3, 5);
    //  std::cout << p2.x() << " " << p2.y() << " " << p2.z() << std::endl;   // does not compile!!

    Point3<double> p3(3, 5, 3);
    Point3<double> p30{ p3 };
    Point3<double> p31;
    p31 = p3;

    Point<double, 4> p4;

    try {
      Point<double, 2> p22{ 3.1, 2.1, 4.0 };
    }
    catch (std::out_of_range & e) {
      std::cout << e.what() << std::endl;
    }
    Point<double, 5> p22{ 3.1, 2.1 };
    std::cout << p22;

    Point3<double> p33{ 1.1, 2., 2. };
    std::cout << p33;

    std::array<float, 5> arr{ 1,2,3,4,5 };
    Point<double, 3> q(arr.begin(), arr.end());
    std::cout << q;
    std::cout << q.x() << " " << q.y() << " " << q.z() << std::endl;
    q.x() = 2;
    q[1] = 4;
    q.z() = 232;
    std::cout << q.x() << " " << q.y() << " " << q.z() << std::endl;

    std::array<float, 5> arr2{ 3,2,1,4,5 };
    Point<double, 4> r(arr2.crbegin(), arr2.crend());
    std::cout << r;


    std::cout << "distance: " << q.distance(p33) << std::endl;

    Vec<double, 3> vv{ 3, 5, 1 };
    std::cout << q + vv;
    std::cout << q - vv;
    std::cout << q - p33;
    std::cout << q.vec();
    std::cout << Vec<double, 3>(q);
  }
    
    
  {
    std::cout << "********************** CARTESIAN COORD. TESTING **************************" << std::endl;
    std::cout << "**************************************************************************" << std::endl;
    CartesianCS<> cs;
    std::cout << cs;

    CartesianCS<double> cs3(Axis::X, Vec3<double>(1, 0, 0), Axis::Y, Vec3<double>(0, 1, 0), Point3<double>(3, 1, 1));
    std::cout << cs3;

    CartesianCS<double> cs4(Axis::Z, Vec3<double>(1, 2, 0), Axis::X, Vec3<double>(0, 0, -2));
    std::cout << cs4;

    cs4.translate(Point3<double>(2, 3, 4));
    std::cout << cs4;

    cs4.translate(Vec3<double>(-2, -3, 4));
    std::cout << cs4;

    cs4.alignTo(Axis::Y, Vec3<double>(0, -3, 0), Axis::X, Vec3<double>(-10, 0, 0));
    std::cout << cs4;

    cs3.rotateInt(Axis::Y, PI / 4);
    std::cout << cs3;

    cs3.rotateInt(Vec3<double>(0,1,0), -PI / 4);
    std::cout << cs3;

    cs3.rotateExt(Vec3<double>(0, 1, 0), PI / 4);
    std::cout << cs3;

    cs3.revolveInt(Axis::Z, PI / 4);
    std::cout << cs3;

    cs3.revolveInt(Axis::Y, PI);
    std::cout << cs3;

    cs3.revolveInt(Vec3<double>(0, 1, 0), -PI);
    std::cout << cs3;

    cs3.revolveExt(Vec3<double>(0, 0, 1), PI);
    std::cout << cs3;

    cs3.revolveExt(Point3<double>(3, 1, 1), Vec3<double>(0, 0, 1), HALF_PI);
    std::cout << cs3;

  }
  
  {
    std::cout << "*********************** RECTANGLE TESTING *******************************" << std::endl;
    std::cout << "*************************************************************************" << std::endl;
    Rectangle<double> obj1;
    std::cout << obj1;
    Rectangle<double> obj2(2.0, Point3<double>(1.0, 0, 0));
    std::cout << obj2;
    Rectangle<double> obj3(2.0, 3.0, Point3<double>(1.0, 1.0, 0));
    std::cout << obj3;
    
    //obj3.scale(-2.5);
    obj3.scale(2.5);

    std::cout << obj3;

    obj2.rotate(Vec3<double>(0, 0, 1.0), PI / 4.0 );
    std::cout << obj2;
    //obj2.vertices(8);
    //std::cout << obj2;

    obj2.rotate(Vec3<double>(0, 0, 1.0), PI / 4.0);
    std::cout << obj2;

    obj2.revolve(Vec3<double>(0, 0, 1.0), PI / 4.0);
    std::cout << obj2;

    obj2.revolve(Vec3<double>(0, 0, -1.0), PI / 4.0);
    std::cout << obj2;

    obj2.revolveSync(Vec3<double>(0, 0, 1.0), PI / 4.0);
    std::cout << obj2;

    obj2.translate(Vec3<double>(1, 2, 3));
    std::cout << obj2;

    std::cout << obj2.perimeter() << "\n";
    std::cout << obj2.area() << "\n";

    std::cout << Rectangle<>::perimeter(1, 4) << "\n";
    std::cout << Rectangle<>::area(2.0, 2.5) << "\n";
    std::cout << obj2.center() << "\n";
    std::cout << obj2.width() << "\n";
    std::cout << obj2.height() << "\n";

    std::cout << "*************************************************************************" << std::endl << std::endl;
  }
  
  
  {
    std::cout << "********************** ELLIPSE TESTING **********************************" << std::endl;
    std::cout << "*************************************************************************" << std::endl;
    Ellipse<double> ell1;
    std::cout << ell1;
    Ellipse<double> ell3(3.0, Length(1.0));
    std::cout << ell3;
    Ellipse<double> ell4(5.0, Eccentricity(0.5), Point3<double>(0, 0, 1.0));
    std::cout << ell4;
    ell4.scale(2);
    std::cout << ell4;

    ell4.rotate(Vec3<double>(0, 0, 1.0), PI / 4.0 );
    std::cout << ell4;
    ell4.vertices(8);
    std::cout << ell4;

    Ellipse<double> ell4b(5.0, Eccentricity(0.5), Point3<double>(0, 0, 1.0));
    std::cout << ell4b;

    ell3.vertices(15);
    std::cout << ell3;

    ell3.rotate(Vec3<double>(0, 0, 1.0), PI / 4.0);
    std::cout << ell3;

    ell3.translate(Vec3<double>(1, 2, 3));
    std::cout << ell3;

    ell3.vertices(10);
    std::cout << ell3;

    std::cout << "*************************************************************************" << std::endl << std::endl;
  }
  
  {
    std::cout << "*********************** CIRCLE TESTING **********************************" << std::endl;
    std::cout << "*************************************************************************" << std::endl;
    Circle<double> cir1;
    std::cout << cir1;
    cir1.scale(0.4);
    std::cout << cir1;

    Circle<double> cir2(2.0, Point3<double>(1.0, 0, 0));
    std::cout << cir2;

    cir2.rotate(Vec3<double>(0, 0, 1.0), PI / 4.0);
    std::cout << cir2;
    cir2.vertices(8);
    std::cout << cir2;
       
    cir2.rotate(Vec3<double>(0, 0, 1.0), PI / 4.0);
    std::cout << cir2;

    cir2.translate(Vec3<double>(1, 2, 3));
    std::cout << cir2;

    cir2.vertices(10);
    std::cout << cir2;

    cir2.scale(10);
    std::cout << cir2;

    std::cout << cir2.perimeter() <<"\n";
    std::cout << cir2.area() << "\n";

    std::cout << Circle<>::perimeter(1) << "\n";
    std::cout << Circle<>::area(2.0) << "\n";
    std::cout << cir2.center() << "\n";
    std::cout << cir2.radius() << "\n";
    //std::cout << cir2.a() << "\n";
    //std::cout << cir2.b() << "\n";
    //std::cout << cir2.e() << "\n";
    //std::cout << cir2.focus() << "\n";

    std::cout << "*************************************************************************" << std::endl << std::endl;
  }


  {
    std::cout << "************************* CONE TESTING **********************************" << std::endl;
    std::cout << "*************************************************************************" << std::endl;
    Cone<double> cone1;
    std::cout << cone1;
    Cone<double> cone2(2.0, 5.0, Point3<double>(2.0, 0, 0));
    std::cout << cone2;
    cone2.rotate(Vec3<double>(0, 0, 1.0), PI / 4.0);
    std::cout << cone2;
    cone2.vertices(11);
    std::cout << cone2;
    cone2.rotate(Vec3<double>(0, 1.0, 0), HALF_PI);
    std::cout << cone2;

     cone2.vertices(15);
    std::cout << cone2;

    cone2.translate(Vec3<double>(1, 0, 3));
    std::cout << cone2;

    cone2.scale(4.0);
    std::cout << cone2;


    std::cout << "*************************************************************************" << std::endl << std::endl;
  }

  {
    std::cout << "************************* CYLINDER TESTING **********************************" << std::endl;
    std::cout << "*****************************************************************************" << std::endl;
    Cylinder<float> cyl1;
    std::cout << cyl1;
    Cylinder<float> cyl2(2.0f, 5.0f, Point3<float>(2.0, 0, 0));
    std::cout << cyl2;

    cyl2.vertices(19);
    std::cout << cyl2;
    cyl2.rotate(Vec3<float>(0, 1.0, 0), static_cast<float>(HALF_PI));
    std::cout << cyl2;

    cyl2.vertices(25);
    std::cout << cyl2;

    cyl2.translate(Vec3<float>(1, 0, 3));
    std::cout << cyl2;

    cyl2.rotate(Vec3<float>(0, 0, 1.0), static_cast<float>(PI / 4.0));
    std::cout << cyl2;


    std::cout << "*************************************************************************" << std::endl << std::endl;
  }
  

  {
    std::cout << "*************************** CUBOID TESTING **********************************" << std::endl;
    std::cout << "*****************************************************************************" << std::endl;
    Cuboid<float> obj1;
    std::cout << obj1;
    Cuboid<float> obj2(2.0f, 5.0f, 1.0f, Point3<float>(2.0, 0, 0));
    std::cout << obj2;

    std::cout << obj2;
    obj2.rotate(Vec3<float>(0, 1.0, 0), static_cast<float>(HALF_PI));
    std::cout << obj2;

    std::cout << obj2;

    obj2.translate(Vec3<float>(1, 0, 3));
    std::cout << obj2;

//    obj2.length(6);
    
    std::cout << "*************************************************************************" << std::endl << std::endl;
  }


  {
    std::cout << "************************* ELLIPSOID TESTING *********************************" << std::endl;
    std::cout << "*****************************************************************************" << std::endl;
    Ellipsoid<double> obj1;
    std::cout << obj1;
    Ellipsoid<double> obj2(3.0, 2.0, 1.0, Point3<double>(3.0, 0, 0));
    std::cout << obj2;
        
    //obj2.vertices(19);
    obj2.vertices(29);
    std::cout << obj2;

    obj2.vertices(69);
    std::cout << obj2;
    
    obj2.translate(Vec3<double>(1, 0, 3));
    std::cout << obj2;

    std::cout << "*************************************************************************" << std::endl << std::endl;
  }


  {
    std::cout << "************************* SPHERE TESTING *********************************" << std::endl;
    std::cout << "*****************************************************************************" << std::endl;
    Sphere<double> obj1;
    std::cout << obj1;
    Sphere<double> obj2(3.0, Point3<double>(3.0, 0, 0));
    std::cout << obj2;

    //obj2.vertices(19);
    obj2.vertices(29);
    std::cout << obj2;

    obj2.vertices(69);
    std::cout << obj2;

    std::cout << obj2.area() << std::endl;
    std::cout << obj2.volume() << std::endl;

    std::cout << "*************************************************************************" << std::endl << std::endl;
  }


  {
    std::cout << "************************* REGULAR POLYGON TESTING *********************************" << std::endl;
    std::cout << "*****************************************************************************" << std::endl;
    RegPolygon<double> obj1( 2);
    std::cout << obj1;

    RegPolygon<double> obj2(4, 5.1, Point3<double>(3.0, 0, 0));
    std::cout << obj2;

    for (size_t i = 3; i <= 10; ++i) {
      RegPolygon<double> obj3(i);
      std::cout << obj3;
    }

    std::cout << obj2.area() << std::endl;
    std::cout << obj2.perimeter() << std::endl;

    std::cout << "*************************************************************************" << std::endl << std::endl;
  }

  return 0;
}