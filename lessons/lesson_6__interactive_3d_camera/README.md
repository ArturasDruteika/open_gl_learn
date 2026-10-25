# Lesson 6 - Interactive 3D camera

In the previous lesson, we went from a flat 2D world to a 3D one by constructing and displaying a pyramid. That is really great, because we learned the core concepts of what is needed to "survive" in a world of 3D rendering.

But in this lesson, we will learn how to navigate this 3D world. By navigating, I mean we have to understand how to interact with by changing the camera viewing angles and positions. So the goal of this lesson is simple: learn the concepts needed to controll the camera in order to truly feel the 3D world.

In this lesson we will learn:

- reading mouse button, mouse movement, and keyboard modifier input using GLFW
- tracking mouse movement using the change in X and Y coordinates
- controlling camera yaw and pitch using mouse movement
- rotating the camera around its local up and right axes
- rolling the camera around its local forward axis
- changing the camera's distance from the object using mouse scroll input
- representing and combining camera rotations using quaternions
- using the camera orientation to orbit around our 3D object
- constructing the view matrix from the camera's position and orientation

At the end of this lesson we will be able to control our camera and view our pyramid from all angles like this:

![Pyramid rotation](assets/camera_control.gif)

# Contents

0. [Project Structure](#0-project-structure)
1. [Input Handling](#1-input-handling)
2. [Mouse Movement Tracking](#2-mouse-movement-tracking)
3. [Yaw and Pitch](#3-yaw-and-pitch)
4. [Local Camera Axes](#4-local-camera-axes)
5. [Camera Roll](#5-camera-roll)
6. [Camera Zoom](#5-camera-zoom)
7. [Quaternion Rotations](#6-quaternion-rotations)
8. [Orbit Camera](#7-orbit-camera)
9. [View Matrix](#8-view-matrix)


# 0. Project Structure

Before moving any forward I want to say that starting from this lesson, we will not contain all of our code in a single `main.cpp` file. From now on, everything we write, we try to separate into files. I wanted to do the same for lesson 5 already, but at the same time that lesson already had a lot of new and often complex topics to learn, so introducing file tree was not an option (though, to be honest, I feel like if I separated that lesson into different files, maybe t would have been easier to understand).

From now on, we have this structure:

    lesson_name/
    |
    |
    --- include/    (contains all .hpp (header) files)
    |
    |
    --- src/        (contains all .cpp (source) files)

This way, we keep separate application (the whole lesson code) parts in different files which will make the project (lesson code) easier to read and understand.

Another thing to mention, that starting from this lesson, I am not going to talk about every single piece of code section. If I did that, every single leesson's readme would take thousands of lines of explanations. The goal for each lesson moving forward is just to cover main ideas that lesson is about.


## 0.1. File Tree

I am excluding all the non `.hpp` and `.cpp` files.


    lesson_6_interactive_3d_camera/
    ├── include/
    │   ├── application.hpp
    │   ├── camera_orbit_controller.hpp
    │   ├── camera_orbit.hpp
    │   ├── mesh_open_gl.hpp
    │   ├── mesh.hpp
    │   ├── pyramid.hpp
    │   ├── renderer.hpp
    │   ├── shader_program.hpp
    │   ├── shape.hpp
    │   ├── vertex.hpp
    │   └── window.hpp
    ├── src/
    │   ├── application.cpp
    │   ├── camera_orbit_controller.cpp
    │   ├── camera_orbit.cpp
    │   ├── main.cpp
    │   ├── mesh_open_gl.cpp
    │   ├── mesh.cpp
    │   ├── pyramid.cpp
    │   ├── renderer.cpp
    │   ├── shader_program.cpp
    │   ├── shape.cpp
    │   └── window.cpp

I have been taught a rule that every file should contain only single class, or classes that are in the same family, so let's analize what each of the files pairs (`.hpp` and `.cpp` counterparts) are responsible for. I will list files by how easy it is to understand them.

- `vertex`: contains all the basic and necessary information that describes a single vertex, i.e. it's position and it's color. Do not forget that there could be way more attributes that a vertex could like normals, tangents, texture coordinates and etc, but for this lesson, we are only going to use position and color.
- `mesh`: this is one of those fundamental concepts that needs to be understood. Mesh represents object's geometry using vertices and indices which define how an object's surface is constructed from triangles.
- `shape`: contains basic properties that all possible shapes must contain in order to exist. It also contains a mesh member (this is one of those design things you have to think about before moving forward, because mesh does not necesseraly need to be a part of a `Shape` class). In this lesson those features are:
    
    - position
    - rotation
    - scale

- `pyramid`: contains a mathematical definition that defines a geometric pyramid.
- `mesh_open_gl`: contains OpenGL specific attrbutes used to render a mesh.
- `camera_orbit`: contains a mathematical definition and rotation rules of a camera object. The idea is that this class expects rotation degrees input and it ouputs a view matrix.
- `camera_orbit_controller`: the entire goal of this class is to convert user input (according to rules) into camera rotation degrees on all 3 axes.
- `shader_program`: contains a class that loads shaders source code and compiles it into a usable shader program.
- `application`: contains a top level class that initializes and manages the lifetime of the application with all of the necessary resources like pyramid, renderer, window. It also sets up all the callbacks that are needed for camera control.
- `window`: this class contains the functionality needed to initialize and manage GLFW window. Entire lifetime of GLFW window happens in this class. This class also processes wondow events, such as:

    - window size change
    - mouse button press
    - cursor position on GLFW window change
    - mouse scroll change

- `renderer`: manages OpenGL rendering process.

As you can see, compared to lesson 5, it seems that this lesson is way more organized, and that should be the rule we all follow when creating applications. I always say that if possible, separation of concern should be followed (but trust me when I say this that I often stumble on forgetting this rule myself).

A great way to manage you project is to follow [SOLID](https://en.wikipedia.org/wiki/SOLID) principles, especially the "separation of concern" part.

But, even though lesson 6 code looks organized, there are still a lot of classes that do many things instead of a single one (which already breaks the SOLID principles). This is due to the fact, that the goal of this lesson is not to teach you how to write a good code, rather lessons goal is to lay a strong knowledge groundwork so that we could understand how rendering works. Because of it, I cannot spend 10x time writing a perfect code that would do exactly the same as the current one does.


# 1. Input handling

Before controlling camera we first have to learn how to read input from keyboard and mouse. Do not think about keyboard and mouse for controlling camera only, because with these input devices we can actually control a lot more than you think, for example:

- text input
- tracking which buttons are being pressed
- mouse movement
- scrolling

Great for us that GLFW provides us with the ability to read and enter this vital part that is crucial for an interactive graphical or game engine.

## 1.1. Input Reading 

Notice, that in the previous sentence, I said that "GLFW provides" and not "OpenGL" provides. Do not stumble on this misinterpretation as I once was doing. Input reading has nothing to do with OpenGL. To repeat myself from the first lesson, OpenGL is a graphics library or graphics API, designed to render pixel colors according to set rules, while GLFW is a graphics library framework, designed for handling the window itself. You can think that OpenGL does not understand what a window is, or in, in other way, you can think that it is "blind" towards the window concept.

Since GLFW is the one responsble for the window, it is also responsible for catching the keyboard and mouse events that happen while the window is in active state.


### 1.1.1. Which Window Events Do We Need?

Before moving forward, let's first understand which window events will be needed. I mean if you look at your keyboard, there are many buttons, your mouse also has different buttons, it also has a scroll. Each of these is a separate GLFW window. Also, in order to control the camera, we will need to track the mouse position on the GLFW window. Each (even a tiny) mouse position change is a GLFW window event. Another also, the GLFW window, when we start the application, has a predefined height and width. Most of the applications nowadays provide the ability to change the window size. Guess what, every (even a tiny) window size change is a GLFW event.

So to repeat, for this lesson we will need these events:

- window resize events (framebuffer events)
- cursor position events 
- mouse button press events
- mouse scroll events


### 1.1.2. Input Reading Using GLFW

```C++
// window.hpp
static void framebuffer_size_callback(GLFWwindow* window,
                                      int width,
                                      int height);

static void mouse_button_callback(GLFWwindow* window,
                                  int button,
                                  int action,
                                  int mods);

static void cursor_position_callback(GLFWwindow* window,
                                     double x,
                                     double y);

static void scroll_callback(GLFWwindow* window,
                            double x_offset,
                            double y_offset);

...

// Pointer to the GLFW window object
GLFWwindow* m_window;

// -----------------------------------------------------------------

// window.cpp

// Register GLFW window callables
glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
glfwSetMouseButtonCallback(m_window, mouse_button_callback);
glfwSetCursorPosCallback(m_window, cursor_position_callback);
glfwSetScrollCallback(m_window, scroll_callback);

```

This is how we register window events callbacks using GLFW API. Let's analyze block by block to understand the full picture here.

In the purest form, GLFW constantly tracks events that happen on it's window. Whether it is a mouse position change, window size change or any other event. Even if we did not attach any callbacks, GLFW system still keeps tracking all events. With this functions `glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);` and others, we just tell GLFW system that "hey, any event that is a window size change, please call back the the `framebuffer_size_callback`". Analogous is the other callback registering function calls.

               OPERATING SYSTEM
                      |
                      | Event occurs
                      v
               +-------------+
               | GLFW SYSTEM |
               |             |
               | Processes   |
               | the event   |
               +-------------+
                      |
                      v
             Framebuffer resized
                      |
                      v
            Is callback registered?
                      |
                +-----+-----+
                |           |
               YES          NO
                |           |
                v           v
         Invoke callback   No callback
                |          invoked
                v
    framebuffer_size_callback()
                |
                v
        Your code executes

In general, this is how a callback system works. If you attach one, on the next instance of that event your own function will be invoked.


## 1.2. Callback Functions

As you saw earlier, to each of the window event callback set, we had to tell GLFW which specific our function should be called on which GLFW event, so now we have:

```C++
                             Calls
window size change      -------------->     framebuffer_size_callback()
mouse button press      -------------->     mouse_button_callback()
cursor position change  -------------->     cursor_position_callback()
mouse scroll            -------------->     scroll_callback()
```

### 1.2.1. Definitions

First, lets understand each callback function definition and it's signature.

Let's take this example:

```C++
static void framebuffer_size_callback(GLFWwindow* window,
                                      int width,
                                      int height);
```

Why does it have to have these parameters? For me, the thing that I first did not understand was the reason of why was the `GLFWwindow* window` parameter needed.

GLFW `window` is needed, because you have to tell GLFW onto which GLFW window will the callback be registered. Know that a single GLFW application can have multiple GLFW windows.

![2 GLFW windows](assets/glfw_2_windows.png)

You can setup callbacks so that different window would have different callbacks.

WHy is `int width` and `int height` needed? The reason is because GLFW system callbacks our function it receives width and height values, so our function also has to have at least these 2 parameters. 

It does not mean that `framebuffer_size_callback` has to have only width and height parameters, it can have as many as you want, but window, width and height (in that specific order) must be included.

Analogous to `framebuffer_size_callback` parameters, our other callback functions have also parameters, that glfw callbacks those functions with.


### 1.2.2. Declaration

Now that we have a callbacks attached to GLFW system, we need to understand what our function definitions of those callbacks look like and why they ar ethe way they are.

Let's analyze them 1 by 1:

```C++
// window.hpp declarations
using FramebufferSizeCallback = std::function<void(int, int)>;

...

static void framebuffer_size_callback(GLFWwindow* window,
                                      int width,
                                      int height);

...

void on_framebuffer_size(int width, int height);

...

int m_framebuffer_width;
int m_framebuffer_height;

FramebufferSizeCallback m_framebuffer_size_callback;

...

GLFWwindow* m_window;


// window.cpp definitions

void Window::on_framebuffer_size(int width,
                                 int height)
{
    m_framebuffer_width = width;
    m_framebuffer_height = height;

    if (m_framebuffer_size_callback)
        m_framebuffer_size_callback(width, height);
}

void Window::framebuffer_size_callback(GLFWwindow* window,
                                       int width,
                                       int height)
{
    Window* self_instance = get_window(window);

    if (self_instance)
        self_instance->on_framebuffer_size(width, height);
}

```

This is what happens when GLFW calbacks our function. I don't know if there is a best approach to explaining this, but I try doing it from the bottom up.

At first we have a definition of `framebuffer_size_callback()` function. As said before, this is the the exact start of how we start processing frambuffer callback. 

What you need to notice is that this function (in the declaration part (`window.hpp`)) is a [static](https://www.geeksforgeeks.org/cpp/static-member-function-in-cpp/). It needs to be `static` because GLFW accepts only static function declarations when registering callbacks.

In the body of `framebuffer_size_callback()`, first, we get a pointer to the window that we registered in the callback, because we want to use the actual window object that we created earlier (do not forget that this function, even though is in `Window` class, but since it is static, it cannot contain any that object specific logic).

```C++
Window* self_instance = get_window(window);
```

After it, an object specific function `on_framebuffer_size()` is called. Notice, that because this is a static function, we have to first get the window object and call this function from the object.

```C++
void Window::on_framebuffer_size(int width,
                                 int height)
{
    m_framebuffer_width = width;
    m_framebuffer_height = height;

    if (m_framebuffer_size_callback)
        m_framebuffer_size_callback(width, height);
}
```

Finally, we reached an object specific member function. This is window specific code that is executed for each window instance created if the framebuffer callback was registered for that instance. 

As you can see, this is also, a very simple function. What we do, we just store the new width and height parameters. There can be many reasons why we want to store window dimensions, one of them being a getter functions for dimensions. For a more complex systems, width and height members can be used for some conditions such as "do this, if window height is this" and etc.

Also, `on_framebuffer_size()` function has a callback in itself.

```C++
if (m_framebuffer_size_callback)
    m_framebuffer_size_callback(width, height);
```

Before answering "why is this callback needed?" question, first, let's understand what `m_framebuffer_size_callback` is.

If you look at the `window.hpp` file and look for `m_framebuffer_size_callback` you can notice that is of type `FramebufferSizeCallback` which in itself is a:

```C++
using FramebufferSizeCallback = std::function<void(int, int)>;
```

[`std::function<...>(...)`](https://www.geeksforgeeks.org/cpp/std-function-in-cpp/) makes function a callable. This allows us to do 1 importatnt thing: it allows us to register another callback to this specific function callable (`m_framebuffer_size_callback`). 

As promised, now we need to understand why we there is a callback attached to `m_framebuffer_size_callback`? Why isn't another callback registered directly to `glfwSetFramebufferSizeCallback(...)`?


### 1.2.3. Callback on Callback

In the `Window` class you can notice that there is 1 additional function:

```C++
// window.hpp
void set_framebuffer_size_callback(FramebufferSizeCallback callback);

// window.cpp
void Window::set_framebuffer_size_callback(FramebufferSizeCallback callback)
{
    m_framebuffer_size_callback = std::move(callback);
}
```

And if we look at the `application.cpp` file, we can find these lines:

```C++
m_window.set_framebuffer_size_callback(
    [this](int width, int height)
    {
        m_renderer.set_viewport(width, height);
    }
);
```

These 2 functions is what allows us to to control the OpenGL's viewport size according to GLFW window size. In lesson 2 I have said that GLFW window size and OpenGL viewport size are related concepts, but 2 different things. 

![](assets/glfw_opengl_size_diagram.png)

Just to remind ourselves, there are 3 varying dimensions that come into play when rendering a scene:

- **GLFW** window defines the size of the application window on your screen.
- **OpenGL viewport** defines the rectangular area of the framebuffer where rendered geometry is mapped.
- **OpenGL framebuffer** is a rendering target that stores pixels colors data. You can think of framebuffer's size as the resolution of your rendered geometry.

Usually, OpenGL renderes geometry into a framebuffer using a viewport that matches (you can also make it so that it would not match, but that's for another lesson) the framebuffer's dimensions. The resulting image in the default framebuffer is then displayed in the GLFW window.