#include "Collider.h"

bool Engine::Collider::Intersects(Engine::BoundingBox& first, Engine::BoundingBox& second)
{
    //for an intersection to occur, it needs to happen on all three axes
    //for performance reasons, we split the check into three parts, if any of them return false, the whole function can imediatelly return false
    //we assume that multiple boxes can have perfectly aligned walls, which we count as an intersection, hence the (<=, >=) instead of (<, >)

    //x axis
    bool intersection = first.GetLeft() <= second.GetRight() && first.GetRight() >= second.GetLeft();

    if (!intersection)
        return false;
    else {
        //y axis
        intersection = first.GetBottom() <= second.GetTop() && first.GetTop() >= second.GetBottom();

        if (!intersection)
            return false;
        else {
            //z axis
            intersection = first.GetFront() <= second.GetBack() && first.GetBack() >= second.GetFront();

            if (!intersection)
                return false;
            else
                return true;
        }
    }
}

bool Engine::Collider::Intersects(Engine::BoundingSphere& first, Engine::BoundingSphere& second)
{
    //check if the sum of radi is greater than the distance between the spheres
    return glm::distance(first.center, second.center) < (first.radius + second.radius);
}

bool Engine::Collider::Intersects(Engine::BoundingBox& box, Engine::BoundingSphere& sphere)
{
    float distance = glm::pow(sphere.radius, 2);

    //x axis
    if (sphere.center.x < box.mins.x)
        distance -= glm::pow(sphere.center.x - box.mins.x, 2);
    else
        if (sphere.center.x > box.maxs.x)
            distance -= glm::pow(sphere.center.x - box.maxs.x, 2);

    //y axis
    if (sphere.center.y < box.mins.y)
        distance -= glm::pow(sphere.center.y - box.mins.y, 2);
    else
        if (sphere.center.y > box.maxs.y)
            distance -= glm::pow(sphere.center.y - box.maxs.y, 2);

    //z axis
    if (sphere.center.z < box.mins.z)
        distance -= glm::pow(sphere.center.z - box.mins.z, 2);
    else
        if (sphere.center.z > box.maxs.z)
            distance -= glm::pow(sphere.center.z - box.maxs.z, 2);

    return distance > 0;
}

/* bool Engine::Collider::Intersects(Engine::Triangle& first, Engine::Triangle& second)
{
    //Moller97 triangle - triangle intersection detection algorithm

    //plane equation for the first triangle
    glm::vec3 firstNormal = glm::cross(first.b.position - first.a.position, first.c.position - first.a.position);
    float firstDistance = glm::dot(-firstNormal, first.a.position);

    //plane equation for the second triangle
    glm::vec3 secondNormal = glm::cross(second.b.position - second.a.position, second.c.position - second.a.position);
    float secondDistance = glm::dot(-secondNormal, second.a.position);

    //distances of first triangle's vertices to the second plane
    float firstD1 = glm::dot(secondNormal, first.a.position) + secondDistance;
    float firstD2 = glm::dot(secondNormal, first.b.position) + secondDistance;
    float firstD3 = glm::dot(secondNormal, first.c.position) + secondDistance;

    //distances of second triangle's vertices to the first plane
    float secondD1 = glm::dot(firstNormal, second.a.position) + firstDistance;
    float secondD2 = glm::dot(firstNormal, second.b.position) + firstDistance;
    float secondD3 = glm::dot(firstNormal, second.c.position) + firstDistance;

    //direction of the line formed by the intersection of the two planes
    glm::vec3 lineDirection = glm::cross(firstNormal, secondNormal);
    
    //projections of first triangle's vertices onto the planes intersection line
    float projectionOntoLine_1a = glm::dot(lineDirection, first.a.position);
    float projectionOntoLine_1b = glm::dot(lineDirection, first.b.position);
    float projectionOntoLine_1c = glm::dot(lineDirection, first.c.position);

    //projections of second triangle's vertices onto the planes intersection line
    float projectionOntoLine_2a = glm::dot(lineDirection, second.a.position);
    float projectionOntoLine_2b = glm::dot(lineDirection, second.b.position);
    float projectionOntoLine_2c = glm::dot(lineDirection, second.c.position);
}*/

bool Engine::Collider::Intersects(Engine::Triangle& first, Engine::Triangle& second)
{
    //A fast triangle to triangle intersection test for collision detection
    //Oren Tropp, Ayellet Tal, Ilan Shimshoni
    //Computer Animationand Virtual Worlds 17(5) 2006, pp 527 - 535

    
    //C1 = first.a.position
    //P1 = first.b.position
    //P2 = first.c.position;

    //D1 = second.a.position;
    //Q1 = second.b.position;
    //Q2 = second.c.position;

    auto sVpsV_2 = [&](float s1, glm::vec3 V1, float s2, glm::vec3 V2) {
        return glm::vec3(s1 * V1[0] + s2 * V2[0], s1 * V1[1] + s2 * V2[1], 0.0f);
    };

    //Tomas Moller's "A Fast Triangle-Triangle Intersection Test" - Journal of Graphics Tools, 2(2), 1997
    auto CoplanarTriangleTriangleTest = [&](glm::vec3 N, glm::vec3 V0, glm::vec3 V1, glm::vec3 V2, glm::vec3 U0, glm::vec3 U1, glm::vec3 U2)
    {
        glm::vec3 A;
        short i0, i1;

        //first project onto an axis-aligned plane, that maximizes the area of the triangles, compute indices: i0,i1.
        A[0] = glm::abs(N[0]);
        A[1] = glm::abs(N[1]);
        A[2] = glm::abs(N[2]);

        if (A[0] > A[1]) {
            if (A[0] > A[2]) {
                //A[0] is greatest
                i0 = 1;      
                i1 = 2;
            }
            else {
                //A[2] is greatest
                i0 = 0;      
                i1 = 1;
            }
        }
        /* A[0]<=A[1] */
        else {
            if (A[2] > A[1]) {
                //A[2] is greatest
                i0 = 0;      
                i1 = 1;
            }
            else {
                //A[1] is greatest
                i0 = 0;      
                i1 = 2;
            }
        }

        auto PointTriangleTest = [&](glm::vec3 V0, glm::vec3 U0, glm::vec3 U1, glm::vec3 U2)
        {
            float a = U1[i1] - U0[i1];
            float b = -(U1[i0] - U0[i0]);
            float c = -a * U0[i0] - b * U0[i1];
            float d0 = a * V0[i0] + b * V0[i1] + c;

            a = U2[i1] - U1[i1];
            b = -(U2[i0] - U1[i0]);
            c = -a * U1[i0] - b * U1[i1];
            float d1 = a * V0[i0] + b * V0[i1] + c;

            a = U0[i1] - U2[i1];
            b = -(U0[i0] - U2[i0]);
            c = -a * U2[i0] - b * U2[i1];
            float d2 = a * V0[i0] + b * V0[i1] + c;

            if (d0 * d1 > 0.0f)
                if (d0 * d2 > 0.0f)
                    return true;
        };

        auto EdgeTriangleTest = [&](glm::vec3 V0, glm::vec3 V1, glm::vec3 U0, glm::vec3 U1, glm::vec3 U2)
        {
            float Ax = V1[i0] - V0[i0];
            float Ay = V1[i1] - V0[i1];

            //Franklin Antonio's "Faster Line Segment Intersection" - Graphics Gems III
            auto EdgeEdgeTest = [&](glm::vec3 V0, glm::vec3 U0, glm::vec3 U1)
            {
                float Bx = U0[i0] - U1[i0];
                float By = U0[i1] - U1[i1];
                float Cx = V0[i0] - U0[i0];
                float Cy = V0[i1] - U0[i1];
                float f = Ay * Bx - Ax * By;
                float d = By * Cx - Bx * Cy;

                if ((f > 0 && d >= 0 && d <= f) || (f < 0 && d <= 0 && d >= f))
                {
                    float e = Ax * Cy - Ay * Cx;

                    if (f > 0)
                        if (e >= 0 && e <= f)
                            return true;
                        else
                            if (e <= 0 && e >= f)
                                return true;
                }
            };

            // test edge U0,U1 against V0 ,V1
            EdgeEdgeTest(V0, U0, U1);

            // test edge U1,U2 against V0, V1
            EdgeEdgeTest(V0, U1, U2);

            //test edge U2,U1 against V0, V1
            EdgeEdgeTest(V0, U2, U0);
        };

        //test all edges of triangle 1 against the edges of triangle 2
        EdgeTriangleTest(V0, V1, U0, U1, U2);
        EdgeTriangleTest(V1, V2, U0, U1, U2);
        EdgeTriangleTest(V2, V0, U0, U1, U2);

        // finally, test if tri1 is totally contained in tri2 or vice versa 
        PointTriangleTest(V0, U0, U1, U2);
        PointTriangleTest(U0, V0, V1, V2);

        return false;
    };

    glm::vec3 distance = first.a.position - second.a.position;

    //determinant computation
    float dp0 = first.b.position.y * first.c.position.z - first.c.position.y * first.b.position.z;
    float dp1 = first.b.position.x * first.c.position.z - first.c.position.x * first.b.position.z;
    float dp2 = first.b.position.x * first.c.position.y - first.c.position.x * first.b.position.y;

    float dq1 = second.b.position.x * dp0 - second.b.position.y * dp1 + second.b.position.z * dp2;
    float dq2 = second.c.position.x * dp0 - second.c.position.y * dp1 + second.c.position.z * dp2;
    float dr = -distance.x * dp0 + distance.y * dp1 - distance.z * dp2;

    float beta1 = dr * dq2;
    float beta2 = dr * dq1;
    bool beta1Legal = (beta2 >= 0) && (beta2 <= dq1 * dq1) && (dq1 != 0);
    bool beta2Legal = (beta1 >= 0) && (beta1 <= dq2 * dq2) && (dq2 != 0);

    glm::vec3 t = glm::vec3(0.0f);
    float dq3 = dq2 - dq1;
    float dr3 = dr - dq1;
    float SF = 0;

    if ((dq1 == 0) && (dq2 == 0))
    {
        if (dr != 0)
            //triangles are on parallel planes
            return false;
        else
        {
            glm::vec3 C2 = first.a.position + first.b.position;
            glm::vec3 C3 = first.a.position + first.c.position;
            glm::vec3 D2 = second.a.position + second.b.position;
            glm::vec3 D3 = second.a.position + second.c.position;

            glm::vec3 normal = glm::cross(first.b.position, first.c.position);

            return CoplanarTriangleTriangleTest(normal, first.a.position, C2, C3, second.a.position, D2, D3);
        }
    }
    else if (!beta1Legal && !beta2Legal)
        //fast reject-all, vertices are on the same side of the triangle plane
        return false;

    else if (beta2Legal && beta1Legal) {
        SF = dq1 * dq2;

        t = sVpsV_2(beta2, second.c.position, (-beta1), second.b.position);
    }

    else if (beta1Legal && !beta2Legal) {
        SF = dq1 * dq3;

        //all betas are multiplied by a positive SF
        beta1 = beta1 - beta2;
        float beta3 = dr3 * dq1;
        t = sVpsV_2(SF - beta3 - beta1, second.b.position, beta3, second.c.position);
    }

    else if (beta2Legal && !beta1Legal) {
        SF = dq2 * dq3;

        //all betas are multiplied by a positive SF
        beta2 = beta1 - beta2;
        float beta3 = dr3 * dq2;

        t = sVpsV_2(SF - beta3, second.b.position, beta3 - beta2, second.c.position);
        glm::vec3 Q1 = second.c.position;
        glm::vec3 Q2 = second.c.position;
        beta1 = beta2;
    }

    glm::vec3  p1, p2;
    glm::vec3 r4 = sVpsV_2(SF, distance, beta1, second.b.position);
    float det1, det2, det3;
    float alpha1, alpha2;
    float gama1, gama2, gama3;

    auto SegmentCollide3 = [&](glm::vec3 q, glm::vec3 r)
    {
        p1[0] = SF * first.b.position.x;
        p1[1] = SF * first.b.position.y;

        p2[0] = SF * first.c.position.x;
        p2[1] = SF * first.c.position.y;

        det1 = p1[0] * q[1] - q[0] * p1[1];
        gama1 = (p1[0] * r[1] - r[0] * p1[1]) * det1;
        alpha1 = (r[0] * q[1] - q[0] * r[1]) * det1;
        bool alpha1_legal = (alpha1 >= 0) && (alpha1 <= (det1 * det1) && (det1 != 0));

        det2 = p2[0] * q[1] - q[0] * p2[1];
        alpha2 = (r[0] * q[1] - q[0] * r[1]) * det2;
        gama2 = (p2[0] * r[1] - r[0] * p2[1]) * det2;
        bool alpha2_legal = (alpha2 >= 0) && (alpha2 <= (det2 * det2) && (det2 != 0));
        det3 = det2 - det1;
        gama3 = ((p2[0] - p1[0]) * (r[1] - p1[1]) - (r[0] - p1[0]) * (p2[1] - p1[1])) * det3;

        if (alpha1_legal) {
            if (alpha2_legal) {
                if (((gama1 <= 0) && (gama1 >= -(det1 * det1))) || ((gama2 <= 0) && (gama2 >= -(det2 * det2))) || (gama1 * gama2 < 0))
                    //12
                    return true;
            }
            else {
                if (((gama1 <= 0) && (gama1 >= -(det1 * det1))) || ((gama3 <= 0) && (gama3 >= -(det3 * det3))) || (gama1 * gama3 < 0))
                    //13
                    return true;
            }
        }
        else
            if (alpha2_legal) {
                if (((gama2 <= 0) && (gama2 >= -(det2 * det2))) || ((gama3 <= 0) && (gama3 >= -(det3 * det3))) || (gama2 * gama3 < 0))
                    //23
                    return true;
            }
                return false;
    
    };

    SegmentCollide3(t, r4);

    return false;
}

bool Engine::Collider::Contains(Engine::BoundingBox& larger, Engine::BoundingBox& smaller)
{
    if (smaller.mins.x > larger.mins.x && smaller.mins.y > larger.mins.y && smaller.mins.z > larger.mins.z)
        if (smaller.maxs.x < larger.maxs.x && smaller.maxs.y < larger.maxs.y && smaller.maxs.z < larger.maxs.z)
            return true;
    
    return false;
}

bool Engine::Collider::Contains(Engine::BoundingSphere& larger, Engine::BoundingSphere& smaller)
{
    if (glm::distance(larger.center, smaller.center) + smaller.radius - larger.radius < 0)
        return true;

    return false;
}

bool Engine::Collider::Contains(Engine::BoundingBox& larger, Engine::BoundingSphere& smaller)
{
    if (glm::distance(larger.mins, smaller.center) > smaller.radius && glm::distance(larger.mins, smaller.center) > smaller.radius)
        return true;
    
    return false;
}

bool Engine::Collider::Contains(Engine::BoundingSphere& larger, Engine::BoundingBox& smaller)
{
    if (glm::distance(smaller.mins, larger.center) < larger.radius && glm::distance(smaller.mins, larger.center) < larger.radius)
        return true;

    return false;
}