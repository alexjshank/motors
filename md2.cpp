#include "graphics.h"
#include "md2.h"

#include <iostream>
#include <fstream>
#include <utility>

using std::cout;
using std::cerr;
using std::endl;

extern int modelsRenderedThisFrame;

Vector Model_MD2::_kAnorms[] = {
#include "Anorms.h"
};

int Model_MD2::_kMd2Ident = 'I' + ('D'<<8) + ('P'<<16) + ('2'<<24);
int Model_MD2::_kMd2Version = 8;


Model_MD2::Model_MD2 (const string &filename) {
	this->type = 1;
	this->name = filename;

	// Open the file
	std::ifstream ifs (filename.c_str (), std::ios::binary);

	if (ifs.fail ())
		throw std::runtime_error ("Couldn't open file");

	// Read header
	ifs.read (reinterpret_cast<char *>(&_header), sizeof (Md2Header_t));

	// Check if ident and version are valid
	if (_header.ident != _kMd2Ident)
		throw std::runtime_error ("Bad file ident");

	if (_header.version != _kMd2Version)
		throw std::runtime_error ("Bad file version");

	// Memory allocation for model data
	_skins = new Md2Skin_t[_header.num_skins];
	_texCoords = new Md2TexCoord_t[_header.num_st];
	_triangles = new Md2Triangle_t[_header.num_tris];
	_frames = new Md2Frame_t[_header.num_frames];
	_glcmds = new int[_header.num_glcmds];

	// Read skin names
	ifs.seekg (_header.offset_skins, std::ios::beg);
	ifs.read (reinterpret_cast<char *>(_skins),	sizeof (Md2Skin_t) * _header.num_skins);

	// Read texture coords.
	ifs.seekg (_header.offset_st, std::ios::beg);
	ifs.read (reinterpret_cast<char *>(_texCoords),	sizeof (Md2TexCoord_t) * _header.num_st);

	// Read triangle data
	ifs.seekg (_header.offset_tris, std::ios::beg);
	ifs.read (reinterpret_cast<char *>(_triangles),	sizeof(Md2Triangle_t) * _header.num_tris);

	// Read frames
	ifs.seekg (_header.offset_frames, std::ios::beg);
	for (int i = 0; i < _header.num_frames; ++i)
	{
		// Memory allocation for the vertices of this frame
		_frames[i].verts = new Md2Vertex_t[_header.num_vertices];

		// Read frame data
		ifs.read (reinterpret_cast<char *>(&_frames[i].scale),		sizeof (Vector));
		ifs.read (reinterpret_cast<char *>(&_frames[i].translate),	sizeof (Vector));
		ifs.read (reinterpret_cast<char *>(&_frames[i].name),		sizeof (char) * 16);
		ifs.read (reinterpret_cast<char *>(_frames[i].verts),		sizeof (Md2Vertex_t) * _header.num_vertices);
	}

	// Read OpenGL commands
	ifs.seekg (_header.offset_glcmds, std::ios::beg);
	ifs.read (reinterpret_cast<char *>(_glcmds),
		sizeof (int) * _header.num_glcmds);

	// Close file
	ifs.close();

	// Setup animation infos
	setupAnimations ();
}


// --------------------------------------------------------------------------
// Model_MD2::~Model_MD2
//
// Destructor.  Free all memory allocated for the model.
// --------------------------------------------------------------------------

Model_MD2::~Model_MD2 ()
{
  delete [] _skins;
  delete [] _texCoords;
  delete [] _triangles;
  delete [] _frames;
  delete [] _glcmds;
}


// --------------------------------------------------------------------------
// Model_MD2::loadTexture
//
// Load a texture from file.
// --------------------------------------------------------------------------

bool Model_MD2::loadTexture (const string &filename) {

	return false;
}


// --------------------------------------------------------------------------
// Model_MD2::setTexture
//
// Set the current texture for the mesh.
// --------------------------------------------------------------------------

void Model_MD2::setTexture (const string &filename) {
  SkinMap::iterator itor;
  itor = _skinIds.find (filename);

  if (itor != _skinIds.end ())
    _tex = itor->second;
  else
    _tex = NULL;
}


// --------------------------------------------------------------------------
// Model_MD2::setupAnimations
//
// Setup animation infos.
// --------------------------------------------------------------------------

void
Model_MD2::setupAnimations ()
{
  string currentAnim;
  Md2Anim_t animInfo = { 0, 0 };

  for (int i = 0; i < _header.num_frames; ++i)
    {
      string frameName = _frames[i].name;
      string frameAnim;

      // Extract animation name from frame name
      string::size_type len = frameName.find_first_of ("0123456789");
      if ((len == frameName.length () - 3) &&
	  (frameName[len] != '0'))
	len++;

      frameAnim.assign (frameName, 0, len);

      if (currentAnim != frameAnim)
	{
	  if (i > 0)
	    {
	      // Previous animation is finished, insert
	      // it and start new animation.
	      _anims.insert (AnimMap::value_type
			     (currentAnim, animInfo));
	    }

	  // Initialize new anim info
	  animInfo.start = i;
	  animInfo.end = i;

	  currentAnim = frameAnim;
	}
      else
	{
	  animInfo.end = i;
	}
    }

  // Insert last animation
  _anims.insert (AnimMap::value_type (currentAnim, animInfo));
}


// --------------------------------------------------------------------------
// Model_MD2::renderFrameImmediate
//
// Render the model for the specified frame, using immediate mode.
// --------------------------------------------------------------------------

void
Model_MD2::renderFrameImmediate (int frame)
{
	modelsRenderedThisFrame++;
	// Compute max frame index
	int maxFrame = _header.num_frames;

	// Check if the frame index is valid
	if ((frame < 0) || (frame >= maxFrame))
		frame = 0;

	// Bind to model's texture

	glBegin (GL_TRIANGLES);
	// Draw each triangle
	for (int i = 0; i < _header.num_tris; ++i)
	{
		// Draw each vertex of this triangle
//		for (int j = 0; j < 3; ++j)
//		{
			Md2Frame_t *pFrame = &_frames[frame];


			Md2Vertex_t *pVert = &pFrame->verts[_triangles[i].vertex[0]];
			Md2TexCoord_t *pTexCoords = &_texCoords[_triangles[i].st[0]];

			glTexCoord2f ((float)(pTexCoords->s) / _header.skinwidth, 1.0f - ((float)(pTexCoords->t) / _header.skinheight));
			//	    glNormal3f(_kAnorms[pVert->normalIndex].x,_kAnorms[pVert->normalIndex].y,_kAnorms[pVert->normalIndex].z);
			Vector v = ((Vector((float)pVert->v[0],(float)pVert->v[1],(float)pVert->v[2]) * pFrame->scale) + pFrame->translate) * _scale;
			glVertex3f(v.x,v.y,v.z);


			pVert = &pFrame->verts[_triangles[i].vertex[1]];
			pTexCoords = &_texCoords[_triangles[i].st[1]];

			glTexCoord2f ((float)(pTexCoords->s) / _header.skinwidth, 1.0f - ((float)(pTexCoords->t) / _header.skinheight));
			//	    glNormal3f(_kAnorms[pVert->normalIndex].x,_kAnorms[pVert->normalIndex].y,_kAnorms[pVert->normalIndex].z);
			v = ((Vector((float)pVert->v[0],(float)pVert->v[1],(float)pVert->v[2]) * pFrame->scale) + pFrame->translate) * _scale;
			glVertex3f(v.x,v.y,v.z);

			pVert = &pFrame->verts[_triangles[i].vertex[2]];
			pTexCoords = &_texCoords[_triangles[i].st[2]];

			glTexCoord2f ((float)(pTexCoords->s) / _header.skinwidth, 1.0f - ((float)(pTexCoords->t) / _header.skinheight));
			//	    glNormal3f(_kAnorms[pVert->normalIndex].x,_kAnorms[pVert->normalIndex].y,_kAnorms[pVert->normalIndex].z);
			v = ((Vector((float)pVert->v[0],(float)pVert->v[1],(float)pVert->v[2]) * pFrame->scale) + pFrame->translate) * _scale;
			glVertex3f(v.x,v.y,v.z);
//		}
	}
	glEnd();
}


// --------------------------------------------------------------------------
// Model_MD2::drawModelItpImmediate
//
// Render the model with frame interpolation, using immediate mode.
// --------------------------------------------------------------------------

void
Model_MD2::drawModelItpImmediate (int frameA, int frameB, float interp)
{
	modelsRenderedThisFrame++;
  // Compute max frame index
  int maxFrame = _header.num_frames - 1;

  // Check if frames are valid
  if ((frameA < 0) || (frameB < 0))
    return;

  if ((frameA > maxFrame) || (frameB > maxFrame))
    return;

  // Bind to model's texture

  glBegin (GL_TRIANGLES);
    // Draw each triangle
    for (int i = 0; i < _header.num_tris; ++i)
      {
	// Draw each vertex of this triangle
	for (int j = 0; j < 3; ++j)
	  {
	    Md2Frame_t *pFrameA = &_frames[frameA];
	    Md2Frame_t *pFrameB = &_frames[frameB];

	    Md2Vertex_t *pVertA = &pFrameA->verts[_triangles[i].vertex[j]];
	    Md2Vertex_t *pVertB = &pFrameB->verts[_triangles[i].vertex[j]];

	    Md2TexCoord_t *pTexCoords = &_texCoords[_triangles[i].st[j]];

	    // Compute final texture coords.
	    GLfloat s = static_cast<GLfloat>(pTexCoords->s) / _header.skinwidth;
	    GLfloat t = static_cast<GLfloat>(pTexCoords->t) / _header.skinheight;

	    glTexCoord2f (s, 1.0f - t);

	    // Compute interpolated normal vector
	    Vector normA = _kAnorms[pVertA->normalIndex];
	    Vector normB = _kAnorms[pVertB->normalIndex];

	    Vector n = normA + (normB - normA) * interp;
	    glNormal3f(n.x,n.y,n.z);

	    // Compute final vertex position
	    Vector vecA, vecB, v;

	    // First, uncompress vertex positions
	    vecA = Vector((float*)pVertA->v) * pFrameA->scale + pFrameA->translate;
	    vecB = Vector((float*)pVertB->v) * pFrameB->scale + pFrameB->translate;

	    // Linear interpolation and scaling
	    v = (vecA + (vecB - vecA) * interp) * _scale;

	    glVertex3f(v.x,v.y,v.z);
	  }
      }
  glEnd();
}


// --------------------------------------------------------------------------
// Model_MD2::renderFrameWithGLcmds
//
// Render the model for the specified frame, using OpenGL commands.
// --------------------------------------------------------------------------

void
Model_MD2::renderFrameWithGLcmds (int frame)
{
	modelsRenderedThisFrame++;
  // Compute max frame index
  int maxFrame = _header.num_frames - 1;

  // Check if the frame index is valid
  if ((frame < 0) || (frame > maxFrame))
    return;

  // Bind to model's texture

  // Pointer to OpenGL commands
  int *pGlcmds = _glcmds;
  int i;

  while ((i = *(pGlcmds++)) != 0)
    {
      if (i < 0)
	{
	  glBegin (GL_TRIANGLE_FAN);
	  i = -i;
	}
      else
	{
	  glBegin (GL_TRIANGLE_STRIP);
	}

      // Parse all OpenGL commands of this group
      for (/* nothing */; i > 0; --i, pGlcmds += 3)
	{
	  // pGlcmds[0] : final S texture coord.
	  // pGlcmds[1] : final T texture coord.
	  // pGlcmds[2] : vertex index to draw

	  Md2Glcmd_t *pGLcmd = reinterpret_cast<Md2Glcmd_t *>(pGlcmds);
	  Md2Frame_t *pFrame = &_frames[frame];
	  Md2Vertex_t *pVert = &pFrame->verts[pGLcmd->index];

	  // Send texture coords. to OpenGL
	  glTexCoord2f (pGLcmd->s, 1.0f - pGLcmd->t);

	  // Send normal vector to OpenGL
	  glNormal3f(_kAnorms[pVert->normalIndex].x,_kAnorms[pVert->normalIndex].y,_kAnorms[pVert->normalIndex].z);

	  // Uncompress vertex position and scale it
	  Vector v = (Vector((float*)pVert->v) + pFrame->translate * pFrame->scale) * _scale;

	  glVertex3f(v.x,v.y,v.z);
	}

      glEnd();
    }
}


// --------------------------------------------------------------------------
// Model_MD2::drawModelItpWithGLcmds
//
// Render the model with frame interpolation, using OpenGL commands.
// --------------------------------------------------------------------------

void
Model_MD2::drawModelItpWithGLcmds (int frameA, int frameB, float interp)
{
	modelsRenderedThisFrame++;
  // Compute max frame index
  int maxFrame = _header.num_frames - 1;

  // Check if frames are valid
  if ((frameA < 0) || (frameB < 0))
    return;

  if ((frameA > maxFrame) || (frameB > maxFrame))
    return;

  // Bind to model's texture

  // Pointer to OpenGL commands
  int *pGlcmds = _glcmds;
  int i;

  while ((i = *(pGlcmds++)) != 0)
    {
      if (i < 0)
	{
	  glBegin (GL_TRIANGLE_FAN);
	  i = -i;
	}
      else
	{
	  glBegin (GL_TRIANGLE_STRIP);
	}

      // Parse all OpenGL commands of this group
      for (/* nothing */; i > 0; --i, pGlcmds += 3)
	{
	  // pGlcmds[0] : final S texture coord.
	  // pGlcmds[1] : final T texture coord.
	  // pGlcmds[2] : vertex index to draw

	  Md2Glcmd_t *pGLcmd = reinterpret_cast<Md2Glcmd_t *>(pGlcmds);

	  Md2Frame_t *pFrameA = &_frames[frameA];
	  Md2Frame_t *pFrameB = &_frames[frameB];

	  Md2Vertex_t *pVertA = &pFrameA->verts[pGLcmd->index];
	  Md2Vertex_t *pVertB = &pFrameB->verts[pGLcmd->index];

	  // Send texture coords. to OpenGL
	  glTexCoord2f (pGLcmd->s, 1.0f - pGLcmd->t);

	  // Compute interpolated normal vector
	  Vector normA = _kAnorms [pVertA->normalIndex];
	  Vector normB = _kAnorms [pVertB->normalIndex];

	  Vector n = normA + (normB - normA) * interp;
	  glNormal3f(n.x,n.y,n.z);

	  // Compute final vertex position
	  Vector vecA, vecB, v;

	  // First, uncompress vertiex positions
	  vecA = (Vector((float*)pVertA->v) * pFrameA->scale) + pFrameA->translate;
	  vecB = (Vector((float*)pVertB->v) * pFrameB->scale) + pFrameB->translate;

	  // Linear interpolation and scaling
	  v = (vecA + ((vecB - vecA)*interp)) * _scale;

	  glVertex3f(v.x,v.y,v.z);
	}

      glEnd();
    }
}




Md2Object::Md2Object (Model_MD2 *model) {
	setModel(model);	
	_scale = 0.02f;
	


}

Md2Object::Md2Object() {
	_scale = 0.02f;
}

Md2Object::~Md2Object() {

}


void
Md2Object::drawObjectItp (bool animated, Md2RenderMode renderMode)
{
  glPushMatrix ();
    // Axis rotation
    glRotatef (rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef (rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef (rotation.z, 0.0f, 0.0f, 1.0f);
	glTranslatef(position.x,position.y,-position.z);
    // Set model scale factor
    _model->setScale (_scale);

    glPushAttrib (GL_POLYGON_BIT);
    glFrontFace (GL_CW);

    // Render the model
    switch (renderMode)
      {
      case kDrawImmediate:
	_model->drawModelItpImmediate (_currFrame, _nextFrame, _interp);
	break;

      case kDrawGLcmds:
	_model->drawModelItpWithGLcmds (_currFrame, _nextFrame, _interp);
	break;
      }
    glFrontFace (GL_CCW);

    // GL_POLYGON_BIT
    glPopAttrib ();
  glPopMatrix ();

  if (animated)
    {
      // Increase interpolation percent
      _interp += _percent;
    }
}


// --------------------------------------------------------------------------
// Md2Object::drawObjectFrame
//
// Draw the MD2 object for the specified frame.
// --------------------------------------------------------------------------

void Md2Object::drawObjectFrame (float frame, Md2RenderMode renderMode)
{
  glPushMatrix ();
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glTranslatef(position.x,position.y,position.z);
	glRotatef (rotation.x-90, 1.0f, 0.0f, 0.0f);
	glRotatef (rotation.y, 0.0f, 1.0f, 0.0f);
	glRotatef (rotation.z, 0.0f, 0.0f, 1.0f);

//	glTranslatef(0,0,3.2);

    // Set model scale factor
    _model->setScale (_scale);

 //   glPushAttrib (GL_POLYGON_BIT);
 //   glFrontFace (GL_CW);

	_model->renderFrameImmediate ((int)frame);

	
	// GL_POLYGON_BIT
//    glPopAttrib ();
  glPopMatrix ();
}


// --------------------------------------------------------------------------
// Md2Object::Animate
//
// Animate the object.  Compute current and next frames, and the
// interpolation percent.
// --------------------------------------------------------------------------

void
Md2Object::animate (int startFrame, int endFrame, float percent)
{
  // _currFrame must range between startFrame and endFrame
  if (_currFrame < startFrame)
    _currFrame = startFrame;

  if (_currFrame > endFrame)
    _currFrame = startFrame;

  _percent = percent;

  // Compute current and next frames.
  if (_interp >= 1.0)
    {
      _interp = 0.0f;
      _currFrame++;

      if (_currFrame >= endFrame)
	_currFrame = startFrame;

      _nextFrame = _currFrame + 1;

      if (_nextFrame >= endFrame)
	_nextFrame = startFrame;
    }
}


void Md2Object::animate (float percent)
{
  // Use the current animation
  animate (_animInfo->start, _animInfo->end, percent);
}


// --------------------------------------------------------------------------
// Md2Object::setModel
//
// Attach mesh model to object.
// --------------------------------------------------------------------------

void Md2Object::setModel (Model_MD2 *model)
{
  _model = model;

  if (_model)
    {
      // Set first animation as default animation
      _animInfo = (Md2Anim_t*)&_model->anims().begin ()->second;
      _currentAnim = _model->anims ().begin ()->first;
    }
}


// --------------------------------------------------------------------------
// Md2Object::setAnim
//
// Set current object animation.
// --------------------------------------------------------------------------

void Md2Object::setAnim (const string &name)
{
  // Try to find the desired animation
  Model_MD2::AnimMap::const_iterator itor;
  itor = _model->anims ().find (name);

  if (itor != _model->anims ().end ())
    {
 //     _animInfo = &itor->second;
      _currentAnim = name;
    }
}
