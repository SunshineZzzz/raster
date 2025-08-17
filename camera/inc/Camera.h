#pragma once

#include "Math.h"

namespace szmath
{
    class Camera
    {
    public:
        // ����ͷλ��
        float3 _eye;
        // �������ע�ӵ�Ŀ���
        float3 _target;
        // ������ֲ�����ϵ�е�+Y��
        float3 _up;
        // ����ֲ�����ϵ��+X��
        float3 _right;
        // ������ֲ�����ϵ��-Z��
        float3 _dir;
        // �۲����
        matrix4 _matView;
        // ͶӰ����
        matrix4 _matProj;
        // �ӿڵĴ�С
        float2 _viewSize;

    public:
        Camera(const float3& target = float3(0, 0, 0), 
            const float3& eye = float3(0, 100, 100), 
            const float3& right = float3(1, 0, 0)) noexcept
        {
            _viewSize = float2(256, 256);

            _matView = szmath::matrix4(1);
            _matProj = szmath::matrix4(1);

            _target = target;
            _eye = eye;
            _dir = normalize(_target - _eye);
            _right = right;
            _up = normalize(cross(_right, _dir));
        }

        float3 getEye() const noexcept
        {
            return _eye;
        }

        void setEye(float3 val) noexcept
        {
            _eye = val;
        }

        float3 getTarget() const noexcept
        {
            return _target;
        }

        void setTarget(float3 val) noexcept
        {
            _target = val;
        }

        void setRight(szmath::float3 val) noexcept
        {
            _right = val;
        }

        float3 getUp() const noexcept
        {
            return _up;
        }

        void setUp(float3 val) noexcept
        {
            _up = val;
        }

        float3 getDir() const noexcept
        {
            return _dir;
        }

        float3 getRight() const noexcept
        {
            return _right;
        }

        void setViewSize(const float2& viewSize) noexcept
        {
            _viewSize = viewSize;
        }

        void setViewSize(float x, float y) noexcept
        {
            _viewSize = float2(x, y);
        }

        float2 getViewSize() noexcept
        {
            return _viewSize;
        }

        void setProject(const matrix4& proj) noexcept
        {
            _matProj = proj;
        }

        const matrix4& getProject() const noexcept
        {
            return _matProj;
        }

        const matrix4& getView() const noexcept
        {
            return _matView;
        }

        void perspective(float fovy, float aspect, float zNear, float zFar) noexcept
        {
            _matProj = szmath::perspective<float>(fovy, aspect, zNear, zFar);
        }

        // ��������ת��Ϊ��������
        bool project(const float4& world, float4& screen) noexcept
        {
            screen = (_matProj * _matView) * world;
            if (screen.w == 0.0f)
            {
                return false;
            }

            // ����͸��ͶӰ����(����ͶӰ����)ת��NDC������ͶӰ����w����1
            // ��Ҫ�Ǳ�����ξ��������
            screen.x /= screen.w;
            screen.y /= screen.w;
            screen.z /= screen.w;

            // map to range 0 - 1
            screen.x = screen.x * 0.5f + 0.5f;
            screen.y = screen.y * 0.5f + 0.5f;
            screen.z = screen.z * 0.5f + 0.5f;

            // map to viewport
            screen.x = screen.x * _viewSize.x;
            screen.y = _viewSize.y - (screen.y * _viewSize.y);

            return true;
        }

        float2 worldToScreen(const float3& world) noexcept
        {
            float4 worlds(world.x, world.y, world.z, 1);
            float4 screens;
            project(worlds, screens);
            return float2(screens.x, screens.y);
        }

        // ��������ת��Ϊ��������
        bool unProject(const float4& screen, float4& world) noexcept
        {
            float4 v;
            v.x = screen.x;
            v.y = screen.y;
            v.z = screen.z;
            v.w = 1.0;

            // map from viewport to 0 - 1
            v.x = (v.x) / _viewSize.x;
            v.y = (_viewSize.y - v.y) / _viewSize.y;

            // map to range -1 to 1
            v.x = v.x * 2.0f - 1.0f;
            v.y = v.y * 2.0f - 1.0f;
            v.z = v.z * 2.0f - 1.0f;

            matrix4 inverse = (_matProj * _matView).inverse();

            v = v * inverse;
            if (v.w == 0.0f)
            {
                return false;
            }

            world = v / v.w;

            return true;
        }

        float3 screenToWorld(const float2& screen) noexcept
        {
            float4 screens(screen.x, screen.y, 0, 1);
            float4 world;
            unProject(screens, world);
            return float3(world.x, world.y, world.z);
        }

        float3 screenToWorld(float x, float y) noexcept
        {
            float4 screens(x, y, 0, 1);
            float4 world;
            unProject(screens, world);
            return float3(world.x, world.y, world.z);
        }

        // �������������������ϵ��Y����ת
        void rotateViewY(float angle) noexcept
        {
            _dir = rotateY<float>(_dir, angle);
            _up = rotateY<float>(_up, angle);
            _right = normalize(cross(_dir, _up));
            // �����������ǰ��Ŀ��ľ��롣�����������ת�������Ǳ��ֲ���ģ���ȷ�������������Ŀ��������Զ
            float len = length(_eye - _target);
            // �����µĹ۲췽��_dir��֮ǰ����ľ���len�����¼������������λ��_eye
            _eye = _target - _dir * len;
            _matView = lookAt(_eye, _target, _up);
        }

        // ����������Լ��ľֲ�����_right��ת
        void rotateViewX(float angle) noexcept
        {
            matrix4 mat(1);
            mat.rotate(angle, _right);
            _dir = _dir * mat;
            _up = _up * mat;
            _right = normalize(cross(_dir, _up));
            float   len = length(_eye - _target);
            _eye = _target - _dir * len;
            _matView = lookAt(_eye, _target, _up);
        }

        void update() noexcept
        {
            _matView = lookAt(_eye, _target, _up);
        }

        Ray createRayFromScreen(int x, int y)
        {
            float4 minWorld;
            float4 maxWorld;

            float4 screen(float(x), float(y), 0, 1);
            float4 screen1(float(x), float(y), 1, 1);

            unProject(screen, minWorld);
            unProject(screen1, maxWorld);

            Ray ray;
            ray.setOrigin(float3(minWorld.x, minWorld.y, minWorld.z));

            float3 dir(maxWorld.x - minWorld.x, maxWorld.y - minWorld.y, maxWorld.z - minWorld.z);
            ray.setDirection(normalize(dir));
            return ray;
        }
    };
}