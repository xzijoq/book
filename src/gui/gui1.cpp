#include <cstdio>
#include <cstdlib>

#define GLM_FORCE_RADIANS 1
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glad/glad.h"

static const int     SCREEN_FULLSCREEN = 0;
static const int     SCREEN_WIDTH = 960;
static const int     SCREEN_HEIGHT = 540;
static SDL_Window*   window = NULL;
static SDL_GLContext maincontext;

static void sdl_die( const char* message )
{
  fprintf( stderr, "%s: %s\n", message, SDL_GetError() );
  exit( 2 );
}

static void APIENTRY openglCallbackFunction( GLenum source, GLenum type,
                                             GLuint id, GLenum severity,
                                             GLsizei       length,
                                             const GLchar* message,
                                             const void*   userParam )
{
  (void)source;
  (void)type;
  (void)id;
  (void)severity;
  (void)length;
  (void)userParam;
  fprintf( stderr, "%s\n", message );
  if ( severity == GL_DEBUG_SEVERITY_HIGH )
  {
    fprintf( stderr, "Aborting...\n" );
    abort();
  }
}

void init_screen( const char* caption )
{
  // Initialize SDL
  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {sdl_die( "Couldn't initialize SDL" );}
  atexit( SDL_Quit );
  SDL_GL_LoadLibrary( NULL );  // Default OpenGL is fine.

  // Request an OpenGL 4.5 context (should be core)
  SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
  // Also request a depth buffer
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
  // Request a debug context.
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG );

  // Create the window
  if ( SCREEN_FULLSCREEN )
  {
    window = SDL_CreateWindow(
        caption, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0,
        SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL );
  }
  else
  {
    window = SDL_CreateWindow( caption, SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                               SCREEN_HEIGHT, SDL_WINDOW_OPENGL );
  }
  if ( window == NULL ) sdl_die( "Couldn't set video mode" );

  maincontext = SDL_GL_CreateContext( window );
  if ( maincontext == NULL ) sdl_die( "Failed to create OpenGL context" );

  // Check OpenGL properties
  printf( "OpenGL loaded\n" );
  gladLoadGLLoader( SDL_GL_GetProcAddress );
  printf( "Vendor:   %s\n", glGetString( GL_VENDOR ) );
  printf( "Renderer: %s\n", glGetString( GL_RENDERER ) );
  printf( "Version:  %s\n", glGetString( GL_VERSION ) );

  // Enable the debug callback
  glEnable( GL_DEBUG_OUTPUT );
  glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
  glDebugMessageCallback( openglCallbackFunction, nullptr );
  glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL,
                         true );

  // Use v-sync
  SDL_GL_SetSwapInterval( 1 );

  // Disable depth test and face culling.
  glDisable( GL_DEPTH_TEST );
  glDisable( GL_CULL_FACE );

  int w, h;
  SDL_GetWindowSize( window, &w, &h );
  glViewport( 0, 0, w, h );
  glClearColor( 0.0f, 0.5f, 1.0f, 0.0f );
}

void attach_shader( GLuint program, GLenum type, const char* code )
{
  GLuint shader = glCreateShader( type );
  glShaderSource( shader, 1, &code, NULL );
  glCompileShader( shader );
  glAttachShader( program, shader );
  glDeleteShader( shader );
}

int main1()
{
  init_screen( "OpenGL 4.5" );

  GLuint texture;
  glCreateTextures( GL_TEXTURE_2D, 1, &texture );
  glTextureStorage2D( texture, 1, GL_RGBA8, 1024, 1024 );

  GLuint image_data[ 960 * 540 ];
  for ( int i = 0; i < 960 * 540; i++ )
  {
    image_data[ i ] = rand();
  }
  glTextureSubImage2D( texture, 0, 0, 0, 960, 540, GL_RGBA, GL_UNSIGNED_BYTE,
                       image_data );

  GLfloat data[ 8 ] = {
      -1, -1, -1, 1, 1, -1, 1, 1,
  };
  GLuint buffer;
  glCreateBuffers( 1, &buffer );
  glNamedBufferStorage( buffer, sizeof( data ), data, 0 );

  int    buffer_index = 0;
  GLuint array;
  glCreateVertexArrays( 1, &array );
  glVertexArrayVertexBuffer( array, buffer_index, buffer, 0,
                             sizeof( GLfloat ) * 2 );

  int position_attrib = 0;
  glEnableVertexArrayAttrib( array, position_attrib );
  glVertexArrayAttribFormat( array, position_attrib, 2, GL_FLOAT, GL_FALSE, 0 );
  glVertexArrayAttribBinding( array, position_attrib, buffer_index );

  GLuint program = glCreateProgram();
  glObjectLabel( GL_PROGRAM, program, -1, "TextureCopy" );

  attach_shader( program, GL_VERTEX_SHADER, R"(
    #version 450 core
    layout(location=0) in vec2 coord;
    void main(void) {
      gl_Position = vec4(coord, 0.0, 1.0);
    }
  )" );

  attach_shader( program, GL_FRAGMENT_SHADER, R"(
    #version 450 core
    readonly restrict uniform layout(rgba8) image2D image;
    layout(location=0) out vec4 color;
    void main(void) {
      color = imageLoad(image, ivec4(gl_FragCoord).xy);
    }
  )" );

  glLinkProgram( program );
  GLint result;
  glGetProgramiv( program, GL_LINK_STATUS, &result );
  if ( result != GL_TRUE )
  {
    char msg[ 10240 ];
    glGetProgramInfoLog( program, 10240, NULL, msg );
    fprintf( stderr, "Linking program failed:\n%s\n", msg );
    abort();
  }

  SDL_Event event;
  bool      quit = false;
  while ( !quit )
  {
    glUseProgram( program );
    glBindImageTexture( 0, texture, 0, false, 0, GL_READ_ONLY, GL_RGBA8 );
    glUniform1i( 0, 0 );
    glBindVertexArray( array );
    glMemoryBarrier( GL_SHADER_IMAGE_ACCESS_BARRIER_BIT );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );

    SDL_GL_SwapWindow( window );
    while ( SDL_PollEvent( &event ) )
    {
      if ( event.type == SDL_QUIT )
      {
        quit = true;
      }
    }
  }
  return 0;
}
