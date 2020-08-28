/* nuklear - public domain */
#include <GL/glew.h>

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear.h"
#include "nuklear_glfw_gl3.h"

void keyboard_event(struct nk_input* in)
{
	if (in->keyboard.keys[NK_KEY_ENTER].down && in->keyboard.keys[NK_KEY_ENTER].clicked)
	{
		printf("Key Enter was pressed!\n");
		
		return;
	} else if (in->keyboard.keys[NK_KEY_TAB].down)
    {
		printf("Key TAB was pressed!\n");
		
		return;	
	} else if (in->keyboard.keys[NK_KEY_SPACE].down)
    {
		printf("Key SPACE was pressed!\n");
		
		return;	
	}
	
	return;	
}

void mouse_event(struct nk_rect   bounds, 
				 struct nk_input* in)
{
	if (in->mouse.buttons[NK_BUTTON_LEFT].down                    &&
		in->mouse.buttons[NK_BUTTON_LEFT].clicked                 &&  
	   (in->mouse.pos.x > bounds.x && in->mouse.pos.x < bounds.w) && 
	   (in->mouse.pos.y > bounds.y && in->mouse.pos.y < bounds.h))
				
	{
		printf("x-coord clicked = %f\n", in->mouse.pos.x);
		printf("y-coord clicked = %f\n", in->mouse.pos.y);
	}
	else if (in->mouse.buttons[NK_BUTTON_RIGHT].down                    &&
		in->mouse.buttons[NK_BUTTON_RIGHT].clicked                 &&  
	   (in->mouse.pos.x > bounds.x && in->mouse.pos.x < bounds.w) && 
	   (in->mouse.pos.y > bounds.y && in->mouse.pos.y < bounds.h))
				
	{
		printf("Messi\n");
	}

	return;				   
}



static void error_callback(int e, const char *d)
{printf("Error %d: %s\n", e, d);}

int main(void)
{
    /* Platform */
    int win_width = 200;
    int win_height = 250;

    /* GLFW */
    struct nk_glfw glfw = {0};
    static GLFWwindow *win;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) 
    {fprintf(stdout, "[GFLW] failed to init!\n");exit(1);}
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    win = glfwCreateWindow(win_width, win_height, "Demo", NULL, NULL);
    glfwMakeContextCurrent(win);

    /* Glew */
    glewExperimental = 1;
    if (glewInit() != GLEW_OK) 
    {fprintf(stderr, "Failed to setup GLEW\n");exit(1);}

	/* create context */
    struct nk_context *ctx = nk_glfw3_init(&glfw, win, NK_GLFW3_INSTALL_CALLBACKS);

    {struct nk_font_atlas *atlas;
    nk_glfw3_font_stash_begin(&glfw, &atlas);
    nk_glfw3_font_stash_end(&glfw);}

    while (!glfwWindowShouldClose(win))
    {
		keyboard_event(&ctx->input);
		
        /* Input */
        glfwPollEvents();
        nk_glfw3_new_frame(&glfw);
		static int motion_X = 0;
        /* GUI */
        if (nk_begin(ctx, "Anything window", nk_rect(0, 0, win_width, win_height), 0))
        {
            nk_layout_row_dynamic(ctx, 120, 1);

            struct nk_command_buffer*canvas = nk_window_get_canvas(ctx);


			if (nk_widget_is_hovered(ctx))
				printf("Yey. The widget was hovered!!\n");
			else
				printf("Not hovered\n");

			struct nk_rect space;
			nk_widget(&space, ctx);
		

			mouse_event(space, &ctx->input);
			
			nk_fill_rect(canvas, space, 2, nk_rgb(160, 160, 160));
        
            nk_fill_circle(canvas, nk_rect(space.x + 20 + motion_X % 100, space.y + 60, 40, 40), nk_rgb(50, 50, 220));
            motion_X++;
            
            nk_stroke_line(canvas, space.x +10, space.y + 100, space.x + 150, space.y + 100, 3.0, nk_rgb(200, 20, 20));
            
            nk_layout_row_dynamic(ctx, 50, 1);
            nk_label(ctx, "Hello world!", NK_TEXT_LEFT);
            
            nk_layout_row_static(ctx, 30, 80, 1);
            if (nk_button_label(ctx, "AnyButton"))
                fprintf(stdout, "button pressed\n");
        }
        nk_end(ctx);

        /* Draw */
        glViewport(0, 0, win_width, win_height);
        glClear(GL_COLOR_BUFFER_BIT);
        nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
        glfwSwapBuffers(win);
    }
    nk_glfw3_shutdown(&glfw);
    glfwTerminate();
    return 0;
}
