/* these headers and some code adapted from http://tfc.duke.free.fr/old/models/md2.htm ... which i guess makes this whole project gpl 2... but this is just for personal use so theres no need to release anything or what not... right... babbling again*/
#include "library.h"
#include "vector.h"
#include <map>
#include <string>

using std::string;


#ifndef __MD2__H__
#define __MD2__H__

// Md2 header
struct Md2Header_t
{
  int ident;          // Magic number, "IDP2"
  int version;        // Md2 format version, should be 8

  int skinwidth;      // Texture width
  int skinheight;     // Texture height

  int framesize;      // Size of a frame, in bytes

  int num_skins;      // Number of skins
  int num_vertices;   // Number of vertices per frame
  int num_st;         // Number of texture coords
  int num_tris;       // Number of triangles
  int num_glcmds;     // Number of OpenGL commands
  int num_frames;     // Number of frames

  int offset_skins;   // offset to skin data
  int offset_st;      // offset to texture coords
  int offset_tris;    // offset to triangle data
  int offset_frames;  // offset to frame data
  int offset_glcmds;  // offset to OpenGL commands
  int offset_end;     // offset to the end of the file
};


// Skin data
struct Md2Skin_t
{
  char name[64];  // Texture's filename
};


// Texture coords.
struct Md2TexCoord_t
{
  short s;
  short t;
};


// Triangle data
struct Md2Triangle_t
{
  unsigned short vertex[3];  // Triangle's vertex indices
  unsigned short st[3];      // Texture coords. indices
};


// Vertex data
struct Md2Vertex_t
{
  unsigned char v[3];         // Compressed vertex position
  unsigned char normalIndex;  // Normal vector index
};


// Frame data
struct Md2Frame_t
{
  // Destructor
  ~Md2Frame_t () {
    delete [] verts;
  }

  Vector scale;        // Scale factors
  Vector translate;    // Translation vector
  char name[16];       // Frame name
  Md2Vertex_t *verts;  // Frames's vertex list
};


// OpenGL command packet
struct Md2Glcmd_t
{
  float s;    // S texture coord.
  float t;    // T texture coord.
  int index;  // Vertex index
};


// Animation infos
struct Md2Anim_t
{
  int start;  // first frame index
  int end;    // last frame index
};


/////////////////////////////////////////////////////////////////////////////
//
// class Model_MD2 -- MD2 Model Data Class.
//
/////////////////////////////////////////////////////////////////////////////


class Model_MD2 : public LibraryObject {
public:
	// Constructors/destructor
	Model_MD2() { 	
		type = 1;
	}
	Model_MD2 (const string &filename);
	~Model_MD2 ();

public:
	// Internal types
	typedef std::map<std::string, int> SkinMap;
	typedef std::map<std::string, Md2Anim_t> AnimMap;

public:
	// Public interface
	bool loadTexture (const string &filename);
	void setTexture (const string &filename);

	void renderFrameImmediate (int frame);
	void drawModelItpImmediate (int frameA, int frameB, float interp);

	void renderFrameWithGLcmds (int frame);
	void drawModelItpWithGLcmds (int frameA, int frameB, float interp);

	void setScale (float scale) { _scale = scale; }

	// Accessors
	const SkinMap &skins () const { return _skinIds; }
	const AnimMap &anims () const { return _anims; }

private:
	// Internal functions
	void setupAnimations ();

private:
	// Member variables

	// Constants
	static Vector _kAnorms[162];
	static int _kMd2Ident;
	static int _kMd2Version;

	// Model data
	Md2Header_t _header;
	Md2Skin_t *_skins;
	Md2TexCoord_t *_texCoords;
	Md2Triangle_t *_triangles;
	Md2Frame_t *_frames;
	int *_glcmds;

	float _scale;
	int _tex;

	SkinMap _skinIds;
	AnimMap _anims;

};


class Md2Object
{
public:
  // Public internal types/enums
  enum Md2RenderMode
    {
      kDrawImmediate = 0,
      kDrawGLcmds,
    };

public:
  // Constructor/destructor
  Md2Object ();
  Md2Object (Model_MD2 *model);
  ~Md2Object ();

public:
  // Public interface
  void drawObjectItp (bool animated, Md2RenderMode renderMode);
  void drawObjectFrame (float frame, Md2RenderMode renderMode);
  void animate (int startFrame, int endFrame, float percent);
  void animate (float percent);

  void setModel (Model_MD2 *model);
  void setScale (float scale) { _scale = scale; }
  void setAnim (const string &name);
  void setPosition (Vector pos) { position = pos; }
  void setRotation (Vector rot) { rotation = rot; }

  // Accessors
  const Model_MD2 *model () const { return _model; }
  float scale () const { return _scale; }
  const string &currentAnim () const { return _currentAnim; }

private:
  // Member variables
  Model_MD2 *_model;
  Vector position,rotation;

  int _currFrame;
  int _nextFrame;
  float _interp;

  float _percent;
  float _scale;

  // Animation data
  Md2Anim_t *_animInfo;
  string _currentAnim;
};

#endif