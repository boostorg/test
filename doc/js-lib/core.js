function include( file, path ) {
    document.write( '<script language="javascript" src="' );
    if( path )
        document.write( path + '/' );
    document.write( file + '" > </script>' );
}

// ************************************************************************** //

function normalize_int( v ) {
    var r = parseInt( v );
    return isNaN( r ) ? 0 : r;
}

// ************************************************************************** //

Object.prototype.is_a = function( type ) {
    return this.constructor == type;
}

// ************************************************************************** //

function funcname( f ) {
    var s = f.toString().match( /function (\w*)/ )[1];

    return (!s || s.length == 0) ? "anonymous" : s;
}

// ************************************************************************** //

include ( 'browser.js', viso_path );
include ( 'array_ex.js', viso_path );

// ************************************************************************** //

function normalize_integer_value( v ) {
    if( v === undefined )
        return 0 ;

    if( v === null )
        return v;

    return parseInt(v) || 0;
}

Pair = function ( x, y ) {
    this.x = normalize_integer_value( x );
    this.y = normalize_integer_value( y );
}

Pair.prototype.add = function( the_shift ) {
    return new Pair(
        this.x + parseInt( the_shift.x ),
        this.y + parseInt( the_shift.y ) );
}

Pair.prototype.toString = function() {
    return '('+this.x+','+this.y+')';
}

// ************************************************************************** //

function Quadruple( x1, x2, x3, x4 ) {
    this.x1 = normalize_integer_value( x1 );
    this.x2 = normalize_integer_value( x2 );
    this.x3 = normalize_integer_value( x3 );
    this.x4 = normalize_integer_value( x4 );
}

Quadruple.prototype.add = function( the_shift ) {
    return new Quadruple(
        this.x1 + parseInt( the_shift.x1 ),
        this.x2 + parseInt( the_shift.x2 ),
        this.x3 + parseInt( the_shift.x3 ),
        this.x4 + parseInt( the_shift.x4 ) );
}

// ************************************************************************** //

// EOF
