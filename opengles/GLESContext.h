#pragma once

#include <GLES2/gl2.h>
#include "EGL/egl.h"

// GLES上下文
class GLESContext
{
public:
    // EGL配置，描述颜色/深度/模板等属性
    EGLConfig m_config;
    // EGL渲染表，通常是窗口关联的绘图表面
    EGLSurface m_surface;
    // EGL上下文，OpenGL ES的状态机
    EGLContext m_context;
    // EGL显示连接，代表与本地窗口系统的连接
    EGLDisplay m_display;
    // Windows窗口句柄
    HWND m_hWnd;
    // Windows设备上下文句柄
    HDC m_hDC;
    // 渲染表面的宽度
    int m_winWidth;
    // 渲染表面的高度
    int m_winHeight;

public:
    GLESContext()
    {
        m_config = nullptr;
        m_display = EGL_NO_DISPLAY;
        m_context = EGL_NO_CONTEXT;
        m_surface = EGL_NO_SURFACE;
        m_hWnd = nullptr;
        m_hDC = nullptr;
        m_winWidth = 0;
        m_winHeight = 0;
    }

    ~GLESContext()
    {
        shutdown();
    }

    bool setup(HWND hWnd, HDC hDC)
    {
        m_hWnd = hWnd;
        m_hDC = hDC;

        shutdown();

        // 告诉EGL希望得到的​​绘图表面
        const EGLint attribs[] =
        {
            // 渲染表面类型，屏幕窗口上
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            // 颜色缓冲区中​​蓝色分量​​占用字节数，8位
            EGL_BLUE_SIZE, 8,
            // 颜色缓冲区中​​绿色分量​​占用字节数，8位
            EGL_GREEN_SIZE, 8,
            // 颜色缓冲区中​​红色分量​​占用字节数，8位
            EGL_RED_SIZE, 8,
            // 颜色缓冲区中​​透明度(Alpha)​​占用字节数，8位
            EGL_ALPHA_SIZE, 8,
            // 深度缓冲区(Z-Buffer)​占用字节数，16 位
            EGL_DEPTH_SIZE, 16,
            EGL_NONE
        };
        EGLint format(0);
        EGLint numConfigs(0);
        EGLint major(0);
        EGLint minor(0);
        EGLint configSize(1);

        // 获取Display​
        m_display = eglGetDisplay(hDC);
        // 初始化EGL 
        eglInitialize(m_display, &major, &minor);
        // ​​选择Config​​
        eglChooseConfig(m_display, attribs, &m_config, configSize, &numConfigs);
        eglGetConfigAttrib(m_display, m_config, EGL_NATIVE_VISUAL_ID, &format);
        // 创建渲染Surface
        m_surface = eglCreateWindowSurface(m_display, m_config, hWnd, nullptr);
        if (!m_surface)
        {
            return  false;
        }

        // 创建OpenGL ES上下文
        EGLint attr[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
        m_context = eglCreateContext(m_display, m_config, nullptr, attr);
        if (!m_context)
        {
            eglDestroySurface(m_display, m_surface);
            eglTerminate(m_display);
            return false;
        }

        // 绑定上下文和Surface
        if (!eglMakeCurrent(m_display, m_surface, m_surface, m_context))
        {
            eglDestroySurface(m_display, m_surface);
            eglTerminate(m_display);
            return false;
        }

        eglQuerySurface(m_display, m_surface, EGL_WIDTH, &m_winWidth);
        eglQuerySurface(m_display, m_surface, EGL_HEIGHT, &m_winHeight);

        return true;
    }

    void shutdown()
    {
        if (m_display != EGL_NO_DISPLAY)
        {
            eglMakeCurrent(m_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            if (m_context != EGL_NO_CONTEXT)
            {
                eglDestroyContext(m_display, m_context);
            }

            if (m_surface != EGL_NO_SURFACE)
            {
                eglDestroySurface(m_display, m_surface);
            }
            eglTerminate(m_display);
        }

        m_display = EGL_NO_DISPLAY;
        m_context = EGL_NO_CONTEXT;
        m_surface = EGL_NO_SURFACE;
    }

    void swapBuffer() const
    {
        eglSwapBuffers(m_display, m_surface);
    }
};