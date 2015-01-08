This folder contains the documentation for the Boost.Test library. 
Any contribution or submission to the library should be accompanied by the corresponding documentation. 

The format of the documentation uses [Quickbook]. 

How to build the documentation
==============================


Install Doxygen
---------------
Part of the documentation needs [http://www.doxygen.org Doxygen]. Download and install it. `doxygen` should be accessible from the terminal.

Download Docbook
----------------
Quickbook needs docbook to be installed. Download and untar the docbook archives:

* Docbook XSL that can be found here: 
* Docbook XML that can be found here:

These two archives are supposed to be untarred to `$docbook_xsl_directory` and `$docbook_xml_directory` respectively. 


Download xsltproc
-----------------
This program is needed by docbook, in order to be able to transform XMLs into HTMLs.
`xsltproc` should be accessible from the command line. 


Construct bjam
--------------

Simply by typing in a terminal:
``
> cd $boost_root
> ./bootstrap.[sh|bat]

``

Build the documentation
-----------------------

``
> cd $boost_root/libs/test/doc
> ../../../b2 -sDOCBOOK_XSL_DIR=$docbook_xsl_directory -sDOCBOOK_DTD_DIR=$docbook_xml_directory
``


Recommendations
---------------
