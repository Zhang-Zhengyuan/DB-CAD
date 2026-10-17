#pragma once
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>

enum OPERATOR_TYPES { OPERATOR_IMPORT = 0, OPERATOR_CONSTRUCTOR, OPERATOR_INTERSECTOR, OPERATOR_BOOLEAN, OPERATOR_DEFEATURE, OPERATOR_UNKNOWN, OPERATOR_SPERATOR = 999 };
enum DEFEATURE_TYPES { DEFEATURE_UNREPEAT = 0, DEFEATURE_REMOVE_SMALL, DEFEATURE_SHELL, DEFEATURE_SPLIT, DEFEATURE_ENVELOPE, DEFEATURE_BREP2CSG };
enum INPUT_MODES { SELECTION, INPUT_SPLINE };
// 注意：BASIC_ENTITIES中非样条基本体需放在BSPLINE_CTRLPTS之前
// 否则将导致进行变换时未修改ENTITY_TREE_ITEM的trans。
enum BASIC_ENTITIES {
    E_CUBE = 0,         // 立方体
    E_SPHERE,           // 球体
    E_CYLINDER,         // 圆柱体
    E_TORUS,            // 圆环体
    E_CONE,             // 圆锥体
    E_PRISM,            // 多棱锥
    E_PYRAMID,          // 金字塔
    E_WIGGLE,           // 摆动体
    E_PLANE,            // 矩形面
    E_PPLANE,           // 平行四边形面
    E_CPLANE,           // 圆形面
    E_SPHERES,          // 球面
    E_PSPHERES,         // 部分球面
    E_CONIC_SIDE,       // 圆锥面
    E_CYLINDER_SIDE,    // 圆柱面
    E_PCONIC,           // 部分圆锥面
    E_TORUSS,           // 完整圆环面
    E_PTORUSS,          // 部分圆环面
    E_LAWS,             // 公式面
    E_LINE,             // 直线
    E_ARC,              // 圆弧
    E_ELLIPSE,          // 椭圆
    E_CONIC,            // 二次线
    E_HELIX,            // 螺旋线
    E_SPIRAL,           // 阿基米德螺旋线
    E_SPRING,           // 弹簧线
    E_LAWC,             // 方程线
    E_BSPLINE_CTRLPTS,  // 控制点b样条
    E_BSPLINE_FIT,      // 拟合b样条
    E_BSPLINE_INTRP,    // 插值b样条
    E_BEZIER,           // 贝塞尔
    E_BSPLINE           // b样条
};


namespace dbcad {
// Session identity only. It is never a native pointer or a persisted UUID.
struct ShapeHandle {
    std::uint64_t value = 0;
    explicit operator bool() const { return value != 0; }
    bool operator==(const ShapeHandle&) const = default;
};
using ShapeList = std::vector<ShapeHandle>;
struct Point3 {
    double values[3]{};
    Point3() = default;
    Point3(double x, double y, double z) : values{x,y,z} {}
    explicit Point3(const double* p) : Point3(p[0],p[1],p[2]) {}
    double x() const { return values[0]; }
    double y() const { return values[1]; }
    double z() const { return values[2]; }
    Point3& operator+=(const Point3& p) { for(int i=0;i<3;++i) values[i]+=p.values[i]; return *this; }
    bool operator==(const Point3&) const = default;
};
inline bool pointsNear(const Point3& a, const Point3& b, double tolerance) {
    double d=0; for(int i=0;i<3;++i) d+=(a.values[i]-b.values[i])*(a.values[i]-b.values[i]);
    return d <= tolerance*tolerance;
}
using HandleGrid = std::vector<std::vector<Point3>>;
// Current editing tools only apply translations; keep their accumulated value explicit.
struct Translation {
    Point3 offset;
    Translation& operator*=(const Translation& t) { offset+=t.offset; return *this; }
    Point3 translation() const { return offset; }
};
enum class BooleanOperation { Unite, Intersect, Subtract };
enum class ShapeKind { Unknown, Body, Face, Edge, Vertex };
struct Color {
    double r=0, g=0, b=0;
    Color() = default;
    Color(double r, double g, double b) : r(r),g(g),b(b) {}
    double red() const { return r; }
    double green() const { return g; }
    double blue() const { return b; }
};
struct Bounds3 {
    Point3 minimum, maximum;
    bool valid=false;
    void include(Point3 p) {
        if(!valid) { minimum=maximum=p; valid=true; return; }
        for(int i=0;i<3;++i) {
            if(p.values[i]<minimum.values[i]) minimum.values[i]=p.values[i];
            if(p.values[i]>maximum.values[i]) maximum.values[i]=p.values[i];
        }
    }
    bool contains(Point3 p,double tolerance=0) const {
        if(!valid) return true;
        for(int i=0;i<3;++i)
            if(p.values[i]<minimum.values[i]-tolerance || p.values[i]>maximum.values[i]+tolerance) return false;
        return true;
    }
};
struct DbConfig { std::string host; int port=7687; std::string username, password; };
}
