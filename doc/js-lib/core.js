function JS() {}

// ************************************************************************** //

JS.include = function( file, path ) {
    document.write( '<script language="javascript" src="' );
    if( path )
        document.write( path + '/' );
    document.write( file + '" > </script>' );
}

// ************************************************************************** //

JS.normalize_int = function( v ) {
    if( v === undefined )
        return 0 ;

    if( v === null )
        return v;

    var r = parseInt( v );
    return isNaN( r ) ? 0 : r;
}

// ************************************************************************** //

JS.funcname = function( f ) {
    var matched = f.toString().match( /function (\w*)/ );
    var s = matched ? matched[1] : null;

    return (!s || s.length == 0) ? "anonymous" : s;
}

// ************************************************************************** //

JS.contain_ltwh = function( lt, wh, pos ) {
    return pos.x > lt.x        && 
           pos.y > lt.y        && 
           pos.x < (lt.x+wh.x) &&
           pos.y < (lt.y+wh.y); 
}
// ************************************************************************** //

JS.include( 'browser.js', viso_path );
JS.include( 'array_ex.js', viso_path );

// ************************************************************************** //

function Pair( x, y ) {
    this.x = JS.normalize_int( x );
    this.y = JS.normalize_int( y );
}

Pair.add = function( p1, p2 ) {
    return new Pair(
        parseInt( p1.x ) + parseInt( p2.x ),
        parseInt( p1.y ) + parseInt( p2.y ) );
}

Pair.prototype.toString = function() {
    return '('+this.x+','+this.y+')';
}

// ************************************************************************** //

function Quadruple( x1, x2, x3, x4 ) {
    this.x1 = JS.normalize_int( x1 );
    this.x2 = JS.normalize_int( x2 );
    this.x3 = JS.normalize_int( x3 );
    this.x4 = JS.normalize_int( x4 );
}

Quadruple.add = function( q1, q2 ) {
    return new Quadruple(
        parseInt( q1.x1 ) + parseInt( q2.x1 ),
        parseInt( q1.x2 ) + parseInt( q2.x2 ),
        parseInt( q1.x3 ) + parseInt( q2.x3 ),
        parseInt( q1.x4 ) + parseInt( q2.x4 ) );
}

// ************************************************************************** //

// EOF
