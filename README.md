# Kross

Embedding of scripting into applications

## Introduction

Kross is a scripting bridge to embed scripting functionality
into an application. It supports QtScript as a scripting interpreter backend.


## Components

The core of Kross provides the framework to deal transparently
with interpreter-backends and offers abstract functionality
to deal with scripts.

The Kross::Manager class is a singleton that provides the
main entry point to deal with the Kross Scripting Framework.

The Kross::Interpreter class is the base for interpreter
implementations. Each scripting backend needs to inherit
its own interpreter and implement it.
The Kross::InterpreterInfo class provides abstract information
about a Interpreter before the interpreter-backend itself is
loaded.

The Kross::Action class is an abstract container to deal with
scripts like a single standalone script file. Each action holds
a reference to by the matching Kross::Interpreter created
Kross::Script instance.

The Kross::ActionCollectionProxyModel class implements a
QSortFilterProxyModel for a Kross::ActionCollectionModel instance
which implements QAbstractItemModel to provide a model for views
on a Kross::ActionCollection instance.

The Kross::ActionCollection class manages collections of
Kross::Action instances.


### Modules

Kross is able to optional dynamic load modules which provide then
additional functionality for scripts.

- Kross::FormModule provides access to UI functionality like dialogs or
  widgets.
- Kross::TranslationModule provides access to the KDE internationalization and
  translation facilities.


### Console Application

The Kross console application provides an executable application
to run by Kross supported scripts from within the command line:

    kross myscript.js


## Backends

Currently, the only backend is a JavaScript backend powered by the QtScript
library (which is part of Qt).


