/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/* $Id: rbgdkglconfig.c,v 1.3 2003/08/20 22:36:02 isambart Exp $ */
/* Gdk::GLConfig
 * Copyright (C) 2003 Vincent Isambart <isambart@netcourrier.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "rbgtkglext.h"

#define _SELF(i)   GDK_GL_CONFIG(RVAL2GOBJ(i))
#define _SCREEN(i) GDK_SCREEN(RVAL2GOBJ(i))

#define RARRAY_TO_ATTRIB_ARRAY(rb_array, array) \
    do { \
        int i; \
        array = ALLOCA_N(int, RARRAY(rb_array)->len+1); \
        for (i=0; i<RARRAY(rb_array)->len; ++i) \
            array[i] = NUM2INT(RARRAY(rb_array)->ptr[i]); \
        array[RARRAY(rb_array)->len] = GDK_GL_ATTRIB_LIST_NONE; \
    } while (0)
    
static VALUE
glconfig_initialize(argc, argv, self)
    int argc;
    VALUE* argv;
    VALUE self;
{
#ifndef GDK_MULTIHEAD_SAFE
    if (argc == 1)
    {
        if (TYPE(argv[0]) == T_ARRAY) {
            int *attrib_list;
            RARRAY_TO_ATTRIB_ARRAY(argv[0], attrib_list);
            G_INITIALIZE(self, gdk_gl_config_new(attrib_list));
        } else {
            G_INITIALIZE(self, gdk_gl_config_new_by_mode(NUM2INT(argv[0])));
        }
    }
    else
#endif /* !defined GDK_MULTIHEAD_SAFE */
#ifdef GDKGLEXT_MULTIHEAD_SUPPORT
    if (argc == 2) {
        if (TYPE(argv[1]) == T_ARRAY) {
            int *attrib_list;
            GdkGLConfig* glconfig;
            
            RARRAY_TO_ATTRIB_ARRAY(argv[0], attrib_list);
            glconfig = gdk_gl_config_new_for_screen(_SCREEN(argv[0]), attrib_list);
            G_INITIALIZE(self, glconfig);
        } else {
            GdkGLConfig* glconfig;

            glconfig = gdk_gl_config_new_by_mode_for_screen(_SCREEN(argv[0]), NUM2INT(argv[1]));
            G_INITIALIZE(self, glconfig);
        }
    }
    else
#endif /* defined GDKGLEXT_MULTIHEAD_SUPPORT */
        rb_raise(rb_eArgError, "wrong number of arguments");

    return Qnil;
}

static VALUE
glconfig_get_attrib(self, attribute)
    VALUE self, attribute;
{
    int value;
    gboolean ok;

    ok = gdk_gl_config_get_attrib(_SELF(self), NUM2INT(attribute), &value);
    if (ok)
        return INT2NUM(value);
    else
        return Qnil;
}

static VALUE
glconfig_get_screen(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_config_get_screen(_SELF(self)));
}

static VALUE
glconfig_get_colormap(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_config_get_colormap(_SELF(self)));
}

static VALUE
glconfig_get_visual(self)
    VALUE self;
{
    return GOBJ2RVAL(gdk_gl_config_get_visual(_SELF(self)));
}

static VALUE
glconfig_get_depth(self)
    VALUE self;
{
    return INT2NUM(gdk_gl_config_get_depth(_SELF(self)));
}

static VALUE
glconfig_get_layer_plane(self)
    VALUE self;
{
    return INT2NUM(gdk_gl_config_get_layer_plane(_SELF(self)));
}

static VALUE
glconfig_get_n_aux_buffers(self)
    VALUE self;
{
    return INT2NUM(gdk_gl_config_get_n_aux_buffers(_SELF(self)));
}

static VALUE
glconfig_get_n_sample_buffers(self)
    VALUE self;
{
    return INT2NUM(gdk_gl_config_get_n_sample_buffers(_SELF(self)));
}

static VALUE
glconfig_is_rgba(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_is_rgba(_SELF(self)));
}

static VALUE
glconfig_is_double_buffered(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_is_double_buffered(_SELF(self)));
}

static VALUE
glconfig_is_stereo(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_is_stereo(_SELF(self)));
}

static VALUE
glconfig_has_alpha(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_has_alpha(_SELF(self)));
}

static VALUE
glconfig_has_depth_buffer(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_has_depth_buffer(_SELF(self)));
}

static VALUE
glconfig_has_stencil_buffer(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_has_stencil_buffer(_SELF(self)));
}

static VALUE
glconfig_has_accum_buffer(self)
    VALUE self;
{
    return CBOOL2RVAL(gdk_gl_config_has_accum_buffer(_SELF(self)));
}

void
Init_gtkglext_gdk_glconfig(void)
{
    /* Gdk::GLConfig */
    VALUE GLConfig = G_DEF_CLASS(GDK_TYPE_GL_CONFIG, "GLConfig", mGdk);

    G_DEF_CONSTANTS(GLConfig, GDK_TYPE_GL_CONFIG_MODE,   "GDK_GL_");
    G_DEF_CONSTANTS(GLConfig, GDK_TYPE_GL_CONFIG_ATTRIB, "GDK_GL_");

    rb_define_method(GLConfig, "initialize",          glconfig_initialize,          -1);
    rb_define_method(GLConfig, "get_attrib",          glconfig_get_attrib,           1);
    rb_define_method(GLConfig, "screen",              glconfig_get_screen,           0);
    rb_define_method(GLConfig, "colormap",            glconfig_get_colormap,         0);
    rb_define_method(GLConfig, "visual",              glconfig_get_visual,           0);
    rb_define_method(GLConfig, "depth",               glconfig_get_depth,            0);
    rb_define_method(GLConfig, "layer_plane",         glconfig_get_layer_plane,      0);
    rb_define_method(GLConfig, "colormap",            glconfig_get_colormap,         0);
    rb_define_method(GLConfig, "n_aux_buffers",       glconfig_get_n_aux_buffers,    0);
    rb_define_method(GLConfig, "n_sample_buffers",    glconfig_get_n_sample_buffers, 0);
    rb_define_method(GLConfig, "rgba?",               glconfig_is_rgba,              0);
    rb_define_method(GLConfig, "double_buffered?",    glconfig_is_double_buffered,   0);
    rb_define_method(GLConfig, "stereo?",             glconfig_is_stereo,            0);
    rb_define_method(GLConfig, "has_alpha?",          glconfig_has_alpha,            0);
    rb_define_method(GLConfig, "has_depth_buffer?",   glconfig_has_depth_buffer,     0);
    rb_define_method(GLConfig, "has_stencil_buffer?", glconfig_has_stencil_buffer,   0);
    rb_define_method(GLConfig, "has_accum_buffer?",   glconfig_has_accum_buffer,     0);
}
