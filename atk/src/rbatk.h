/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************
                                                                                
  rbatk.h -
                                                                                
  $Author: mutoh $
  $Date: 2004/03/02 15:55:20 $
                                                                                
  Copyright (C) 2003 Masao Mutoh
************************************************/
                                                                                
#include "ruby.h"
#include <atk/atk.h>
#include <atk/atk-enum-types.h>
#include <atk/atknoopobject.h>
#include <atk/atknoopobjectfactory.h>
#include "rbgobject.h"


#if defined(G_PLATFORM_WIN32) && !defined(RUBY_ATK_STATIC_COMPILATION)
#  ifdef RUBY_ATK_COMPILATION
#    define RUBY_ATK_VAR __declspec(dllexport)
#  else
#    define RUBY_ATK_VAR extern __declspec(dllimport)
#  endif
#else
#  define RUBY_ATK_VAR extern
#endif
                                                                                
extern void Init_atk_inits();
RUBY_ATK_VAR VALUE mAtk;
