JS.include( 'callback_api.js', viso_path );
JS.include( 'scheduler_api.js', viso_path );

//-----------------------------------------------------------------------------
// object utilities.
//-----------------------------------------------------------------------------

function viso_get( vis_obj_id ) {
    return document.getElementById( vis_obj_id );
}

//-----------------------------------------------------------------------------
// object content
//-----------------------------------------------------------------------------

function viso_set_html( vis_obj, html ) {
    if( !vis_obj ) return;

    html = (html != null && html != undefined) ? html : '';

    if( ie ) {
        vis_obj.innerHTML = html;

        var images = vis_obj.all.tags( "img" );
        for( var i=0;i<images.length; i++ )
            images[i].lyrobj=vis_obj;
    } else if( dom ) {
        while( vis_obj.hasChildNodes() )
            vis_obj.removeChild( vis_obj.firstChild );
    
        var r = vis_obj.ownerDocument.createRange();
        r.selectNodeContents( vis_obj );
        r.collapse( true );
    
        var df = r.createContextualFragment( html );
        vis_obj.appendChild( df );
    }
}

//-----------------------------------------------------------------------------
// object display.
//-----------------------------------------------------------------------------

function viso_remove( vis_obj )  {
    if( !vis_obj ) return;

    vis_obj.style.display = "none";
}

function viso_display( vis_obj ) {
    if( !vis_obj ) return;

    vis_obj.style.display = "";
}

function viso_set_displayed( vis_obj, on_off ) {
    if( !vis_obj ) return;

    if( on_off )
        viso_display( vis_obj );
    else
        viso_remove( vis_obj )
}

function viso_is_displayed( vis_obj ) {
    return vis_obj.style.display != "none";
}

//-----------------------------------------------------------------------------
// object visibility.
//-----------------------------------------------------------------------------

function viso_hide( vis_obj ) {
    if( !vis_obj ) return;

    vis_obj.style.visibility = "hidden";
}

function viso_show( vis_obj ) {
    if( !vis_obj ) return;

    vis_obj.style.visibility = "visible";
}

function viso_inherit_visibility( vis_obj ) {
    if( !vis_obj ) return;

    vis_obj.style.visibility = "inherit";
}

function viso_get_visibility( vis_obj ) {
    return vis_obj.style.visibility;
}

function viso_is_visible( vis_obj ) {
    return viso_get_visibility( vis_obj ) != "hidden";
}

//-----------------------------------------------------------------------------
// object size and positioning.
//-----------------------------------------------------------------------------

function viso_get_left( vis_obj ) {
    return JS.normalize_int( vis_obj.style.left );
}

function viso_set_left( vis_obj, x ) {
    if( !vis_obj ) return;

    viso_move_to( vis_obj, new Pair( x, null ) );
}

function shift_left_by( vis_obj, dx ) {
    if( !vis_obj ) return;

    viso_move_by( vis_obj, new Pair( dx, 0 ) );
}

function viso_get_top( vis_obj ) {
    return JS.normalize_int( vis_obj.style.top );
}

function viso_set_top( vis_obj, y ) {
    if( !vis_obj ) return;

    viso_move_to( vis_obj, new Pair( null, y ) );
}

function shift_top_by( vis_obj, dy ) {
    if( !vis_obj ) return;

    viso_move_by( vis_obj, new Pair( 0, dy ) );
}

function viso_get_right( vis_obj ) {
    return viso_get_left( vis_obj ) + viso_get_width( vis_obj );
}

function viso_get_bottom( vis_obj ) {
    return viso_get_top( vis_obj ) + viso_get_height( vis_obj );
}

function viso_get_width( vis_obj ) {
    return JS.normalize_int( vis_obj.style.width ? vis_obj.style.width : vis_obj.offsetWidth );
}

function viso_set_width( vis_obj, w ) {
    if( !vis_obj ) return;

    vis_obj.style.width = JS.normalize_int( w );
}

function viso_insure_width( vis_obj, w ) {
    if( viso_get_width( vis_obj ) < w )
        viso_set_width( vis_obj, w );
}

function viso_get_height( vis_obj ) {
    return JS.normalize_int( vis_obj.style.height ? vis_obj.style.height : vis_obj.offsetHeight );
}

function viso_set_height( vis_obj, h ) {
    if( !vis_obj ) return;

    vis_obj.style.height = JS.normalize_int( h );
}

function viso_insure_height( vis_obj, h ) {
    if( viso_get_height( vis_obj ) < h )
        viso_set_height( vis_obj, h );
}

function viso_get_sizes( vis_obj ) {
    return new Pair( viso_get_width( vis_obj ), viso_get_height( vis_obj ) );
}

function viso_get_zindex( vis_obj ) {
    return JS.normalize_int( vis_obj.style.zIndex );
}

function viso_set_zindex( vis_obj, z ) {
    if( !vis_obj ) return;

    vis_obj.style.zIndex = z;
}

function viso_get_position( vis_obj )  {
    return new Pair( viso_get_left( vis_obj ), viso_get_top( vis_obj ) );
}

function viso_move_to( vis_obj, new_pos ) {
    if( !vis_obj ) return;

    if( new_pos.x == null ) new_pos.x = viso_get_left( vis_obj );
    if( new_pos.y == null ) new_pos.y = viso_get_top( vis_obj );

    vis_obj.style.left = JS.normalize_int( new_pos.x );
    vis_obj.style.top  = JS.normalize_int( new_pos.y );
}

function viso_move_by( vis_obj, shift ) {
    if( !vis_obj ) return;

    vis_obj.style.left = JS.normalize_int( vis_obj.style.left )+JS.normalize_int( shift.x );
    vis_obj.style.top  = JS.normalize_int( vis_obj.style.top ) +JS.normalize_int( shift.y );
}

function viso_smooth_move( vis_obj, speed, algorithm ) {
    if( !vis_obj ) return;

    var bound_work_func = new Callback( viso_do_smooth_move, null,
                                        vis_obj, algorithm, 
                                        viso_get_position( vis_obj ) );
    TaskScheduler.schedule( bound_work_func, speed );
}

function viso_do_smooth_move( vis_obj, algorithm, orig_position, counter ) {
    var shift = algorithm( counter );

    if( shift == null )
        return false;

    viso_move_to( vis_obj, Pair.add( orig_position, shift ) );

    return true;
}

function viso_get_page_left( vis_obj ) {
    var x = 0;

    do {
        x += JS.normalize_int( vis_obj.offsetLeft );
        vis_obj = vis_obj.offsetParent;
    } while( vis_obj );

    return x;
}

function viso_get_page_top( vis_obj ) {
    var y = 0;

    do {
        y += JS.normalize_int( vis_obj.offsetTop );
        vis_obj = vis_obj.offsetParent;
    } while( vis_obj );

    return y;
}

function viso_get_page_position( vis_obj ) {
    return new Pair( viso_get_page_left( vis_obj ), viso_get_page_top( vis_obj ) );
}

//-----------------------------------------------------------------------------
// object clipping.
//-----------------------------------------------------------------------------

function viso_get_clip_area( vis_obj ) {
    var res = new Quadruple;
    var str = vis_obj.style.clip;
    if( str ) {
        var i;
        i = str.indexOf( "(" );
        res.x2 = JS.normalize_int( str.substring( i + 1, str.length ), 10 );
        i = str.indexOf( " ", i + 1 );
        res.x3 = JS.normalize_int( str.substring( i + 1, str.length ), 10 );
        i = str.indexOf( " ", i + 1 );
        res.x4 = JS.normalize_int( str.substring( i + 1, str.length ), 10 );
        i = str.indexOf( " ", i + 1 );
        res.x1 = JS.normalize_int( str.substring( i + 1, str.length ), 10 );
    }
    return res;
}


function viso_clip( vis_obj, clip_area ) {
    if( !vis_obj ) return;

    vis_obj.style.clip = 'rect( ' + clip_area.x2 + ' ' 
                                  + clip_area.x3 + ' ' 
                                  + clip_area.x4 + ' '
                                  + clip_area.x1 + ' )';
}

function viso_change_clip_by( vis_obj, clip_diff ) {
    if( !vis_obj ) return;

    var clip_area = viso_get_clip_area( vis_obj );

    viso_clip( vis_obj, Quadruple.add( clip_area, clip_diff ) );
}

function viso_get_clip_width( vis_obj ) {
    if( !vis_obj.style.clip )
        return JS.normalize_int( vis_obj.style.width );

    var clip_area = viso_get_clip_area( vis_obj );

    return clip_area.x3 - clip_area.x1;
}

function viso_get_clip_height( vis_obj ) {
    if( !vis_obj.style.clip )
        return JS.normalize_int( vis_obj.style.height );

    var clip_area = viso_get_clip_area( vis_obj );

    return clip_area.x4 - clip_area.x2;
}

//-----------------------------------------------------------------------------
// object scrolling.
//-----------------------------------------------------------------------------

function viso_scroll_to( vis_obj, x, y, bound ) {
    if( !vis_obj ) return;

    var clip_area = viso_get_clip_area( vis_obj );
    var dx = x - clip_area.x1;
    var dy = y - clip_area.x2;

    return viso_scroll_by( vis_obj, dx, dy, bound );
}

function viso_scroll_by( vis_obj, dx, dy, bound ) {
    if( !vis_obj ) return;

    dx = JS.normalize_int( dx );
    dy = JS.normalize_int( dy );

    if( dx == 0 && dy == 0 )
        return true;

    var clip_area = viso_get_clip_area( vis_obj );
    if( bound ) {
        var sizes = viso_get_sizes( vis_obj );

        dx = Math.max( dx, -clip_area.x1 );
        dx = Math.min( dx, sizes.x - clip_area.x3 );

        dy = Math.max( dy, -clip_area.x2 );
        dy = Math.min( dy, sizes.y - clip_area.x4 );

        if( dx == 0 && dy == 0 )
            return false;
    }
    viso_clip( vis_obj, Quadruple.add( clip_area, new Quadruple( dx, dy, dx, dy ) ) );
    viso_move_by( vis_obj, new Pair( -dx, -dy ) );

    return true;
}

function viso_smooth_scroll( vis_obj, speed, algorithm ) {
    if( !vis_obj ) return;

    var clip_area = viso_get_clip_area( vis_obj );

    var bound_work_func = new Callback( viso_do_smooth_scroll, null,
                                         vis_obj, algorithm, 
                                         new Pair( clip_area.x1, clip_area.x2 ) );
    TaskScheduler.schedule( bound_work_func, speed );
}

function viso_do_smooth_scroll( vis_obj, algorithm, orig_position, counter ) {
    var shift = algorithm( counter );

    var new_pos = Pair.add( orig_position, shift );

    return viso_scroll_to( vis_obj, new_pos.x, new_pos.y, true );
}

//-----------------------------------------------------------------------------
// object background.
//-----------------------------------------------------------------------------

function viso_set_bg_color( vis_obj, color ) {
    if( !vis_obj ) return;

    vis_obj.style.backgroundColor = color;
}

function viso_set_bg_image( vis_obj, src ) {
    if( !vis_obj ) return;

    vis_obj.style.backgroundImage = "url( " + src + " )";
}

//-----------------------------------------------------------------------------
// events.handling.
//-----------------------------------------------------------------------------

function viso_add_event_handler( event_id, handler, vis_obj ) {
    if( !(handler instanceof Callback ) )
        handler = new Callback( handler );

    if( !vis_obj )
        vis_obj = document;

    if( ie ) {
        event_id            = "on" + event_id;
        var wrapped_handler = function() { handler.run( window.event ); };

        vis_obj.attachEvent( event_id, wrapped_handler );
    }
    else if( dom ) {
        var wrapped_handler = function( e ) { handler.run( e ); };
        vis_obj.addEventListener( event_id, wrapped_handler, false );
    }

    return wrapped_handler;
}

function viso_remove_event_handler( event_id, handler, vis_obj ) {
    if( !vis_obj )
        vis_obj = document;

    if( ie ) {
        event_id = "on" + event_id;

        vis_obj.detachEvent( event_id, handler );
    }
    else if( dom ) {
        vis_obj.removeEventListener( event_id, handler, false );
    }
}

function viso_event_offset_x( e ) {
    return ie ? e.offsetX : e.layerX;
}

function viso_event_offset_y( e ) {
    return ie ? e.offsetY : e.layerY;
}

function viso_event_target( e ) {
    return ie ? e.srcElement : e.target;
}

function viso_event_to_target( e ) {
    return ie ? e.toElement : e.relatedTarget;
}

// EOF
