include ( 'callback_api.js', viso_path );
include ( 'scheduler_api.js', viso_path );

//-----------------------------------------------------------------------------
// object utilities.
//-----------------------------------------------------------------------------

function viso_get( vis_obj_id ) {
    if( ns4 )
        return viso_find( vis_obj_id, document );
    if( ie )
        return eval( 'document.all.' + vis_obj_id );
    if( dom )
        return document.getElementById( vis_obj_id );

    return null;
}

function viso_find( vis_obj_id, doc ) {
    for( var i = 0; i < doc.objs.length; i++ ) {
        var vis_obj = doc.objs[i];

        if( vis_obj.name == vis_obj_id )
            return vis_obj;

        if( vis_obj.document.objs.length > 0 ) {
            vis_obj = viso_find( vis_obj_id, vis_obj.document );
            if( vis_obj != null )
                return vis_obj;
        }
    }
    return null;
}

function viso_get_parent( vis_obj ) {
    if( ns4 )
       alert( "to be impl")
    else if (ie)
        return vis_obj.parentElement;
    else if (dom)
        return vis_obj.parentNode;

    return null;
};


function viso_get_id( vis_obj ) {
    if( ns4 )
        return vis_obj.name;
    if( ie || dom )
        return vis_obj.id;

  return null;
}

//-----------------------------------------------------------------------------
// object style
//-----------------------------------------------------------------------------

function viso_get_style(vis_obj) {
    if( ns4 )
        return vis_obj;
    else if( ie || dom )
        return vis_obj.style;

    return null;
}

//-----------------------------------------------------------------------------
// object content
//-----------------------------------------------------------------------------

function viso_set_html( vis_obj, html ) {
    if( !vis_obj ) return;

    html = (html != null && html != undefined) ? html : '';

    if( ns4 ) {
        vis_obj.document.open();
        vis_obj.document.write(html);
        vis_obj.document.close();

        for( var i=0; i<vis_obj.document.images.length; i++ )
            vis_obj.document.images[i].lyrobj=vis_obj;

        for( var i=0; i<vis_obj.document.links.length;  i++ )
            vis_obj.document.links[i].lyrobj=vis_obj;

    } else if( ie ) {
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

    viso_get_style( vis_obj ).display = "none";
}

function viso_display( vis_obj ) {
    if( !vis_obj ) return;

    viso_get_style( vis_obj ).display = "";
}

function viso_set_displayed( vis_obj, on_off ) {
    if( !vis_obj ) return;

    if( on_off )
        viso_display( vis_obj );
    else
        viso_remove( vis_obj )
}

function viso_is_displayed( vis_obj ) {
    return viso_get_style( vis_obj ).display != "none";
}

//-----------------------------------------------------------------------------
// object visibility.
//-----------------------------------------------------------------------------

function viso_hide( vis_obj ) {
    if( !vis_obj ) return;

    if( ns4 )
        vis_obj.visibility       = "hide";
    else if( ie || dom )
        vis_obj.style.visibility = "hidden";
}

function viso_show( vis_obj ) {
    if( !vis_obj ) return;

    if( ns4 )
        vis_obj.visibility       = "show";
    else if( ie || dom )
        vis_obj.style.visibility = "visible";
}

function viso_inherit_visibility( vis_obj ) {
    if( !vis_obj ) return;

    viso_get_style( vis_obj ).visibility = "inherit";
}

function viso_get_visibility( vis_obj ) {
    if( ns4 ) {
        if( vis_obj.visibility == "show" )
            return "visible";
        if( vis_obj.visibility == "hide" )
            return "hidden";
        return vis_obj.visibility;
    }

    if( ie || dom )
        return vis_obj.style.visibility;

    return "";
}

function viso_is_visible( vis_obj ) {
    return viso_get_visibility( vis_obj ) != "hidden";
}

//-----------------------------------------------------------------------------
// object size and positioning.
//-----------------------------------------------------------------------------

function viso_get_left( vis_obj ) {
    return normalize_int( viso_get_style( vis_obj ).left );
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
    return normalize_int( viso_get_style( vis_obj ).top );
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
    if( ns4 ) {
        if( vis_obj.document.width )
            return normalize_int( vis_obj.document.width );
        else
            return normalize_int( vis_obj.clip.right ) - normalize_int( vis_obj.clip.left );
    }
    if( ie || dom ) {
        if( vis_obj.style.width )
            return normalize_int( vis_obj.style.width );
        else
            return normalize_int( vis_obj.offsetWidth );
    }
    return -1;
}

function viso_set_width( vis_obj, w ) {
    if( !vis_obj ) return;

    if( ns4 ) {
        vis_obj.document.width = normalize_int( w );
    }
    if( ie || dom ) {
        vis_obj.style.width = normalize_int( w );
    }
    return -1;
}

function viso_insure_width( vis_obj, w ) {
    if( viso_get_width( vis_obj ) < w )
        viso_set_width( vis_obj, w );
}

function viso_get_height( vis_obj ) {
    if( ns4 ) {
        if( vis_obj.document.height )
            return normalize_int( vis_obj.document.height );
        else
            return normalize_int( vis_obj.clip.bottom ) - normalize_int( vis_obj.clip.top );
    }
    if( ie || dom ) {
        if( vis_obj.style.height )
            return normalize_int( vis_obj.style.height );
        else
            return normalize_int( vis_obj.offsetHeight );
    }
    return -1;
}

function viso_set_height( vis_obj, h ) {
    if( !vis_obj ) return;

    if( ns4 ) {
        vis_obj.document.height = normalize_int( h );
    }
    if( ie || dom ) {
        vis_obj.style.height = normalize_int( h );
    }
    return -1;
}

function viso_insure_height( vis_obj, h ) {
    if( viso_get_height( vis_obj ) < h )
        viso_set_height( vis_obj, h );
}

function viso_get_sizes( vis_obj ) {
    return new Pair( viso_get_width( vis_obj ), viso_get_height( vis_obj ) );
}

function viso_get_zindex( vis_obj ) {
    return normalize_int( viso_get_style( vis_obj ).zIndex );
}

function viso_set_zindex( vis_obj, z ) {
    if( !vis_obj ) return;

    viso_get_style( vis_obj ).zIndex = z;
}

function viso_get_position( vis_obj )  {
    return new Pair( viso_get_left( vis_obj ), viso_get_top( vis_obj ) );
}

function viso_move_to( vis_obj, new_pos ) {
    if( !vis_obj ) return;

    if( new_pos.x == null ) new_pos.x = viso_get_left( vis_obj );
    if( new_pos.y == null ) new_pos.y = viso_get_top( vis_obj );

    if( ns4 )
        vis_obj.moveTo( normalize_int( new_pos.x ), normalize_int( new_pos.y ) );
    else if( ie || dom ) {
        vis_obj.style.left = normalize_int( new_pos.x );
        vis_obj.style.top  = normalize_int( new_pos.y );
    }
}

function viso_move_by( vis_obj, shift ) {
    if( !vis_obj ) return;

    if( ns4 )
        vis_obj.moveBy( normalize_int( shift.x ), normalize_int( shift.y ) );
    else if( ie || dom ) {
        vis_obj.style.left = normalize_int( vis_obj.style.left )+normalize_int( shift.x );
        vis_obj.style.top  = normalize_int( vis_obj.style.top ) +normalize_int( shift.y );
    }
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

    viso_move_to( vis_obj, orig_position.add( shift ) );

    return true;
}

function viso_get_page_left( vis_obj ) {
    if( ns4 )
        return normalize_int( vis_obj.pageX );
    if( ie || dom ) {
        var x = 0;
        while ( vis_obj.offsetParent != null ) {
            x += normalize_int( vis_obj.offsetLeft );
            vis_obj = vis_obj.offsetParent;
        }
        x += normalize_int( vis_obj.offsetLeft );
        return x;
    }
    return -1;
}

function viso_get_page_top( vis_obj ) {
    if( ns4 )
        return normalize_int( vis_obj.pageY );
    if( ie || dom ) {
        var y = 0;
        while ( vis_obj.offsetParent != null ) {
            y += normalize_int( vis_obj.offsetTop );
            vis_obj = vis_obj.offsetParent;
        }
        y += normalize_int( vis_obj.offsetTop );
        return y;
    }
    return -1;
}

function viso_get_page_position( vis_obj ) {
    return new Pair( viso_get_page_left( vis_obj ), viso_get_page_top( vis_obj ) );
}

//-----------------------------------------------------------------------------
// object clipping.
//-----------------------------------------------------------------------------

function viso_get_clip_area( vis_obj ) {
    if( ns4 )
        return new Quadruple( vis_obj.clip.left, 
                              vis_obj.clip.top,
                              vis_obj.clip.right,
                              vis_obj.clip.bottom );
    if( ie || dom ) {
        var res = new Quadruple;
        var str = vis_obj.style.clip;
        if( str ) {
            var i;
            i = str.indexOf( "(" );
            res.x2 = normalize_int( str.substring( i + 1, str.length ), 10 );
            i = str.indexOf( " ", i + 1 );
            res.x3 = normalize_int( str.substring( i + 1, str.length ), 10 );
            i = str.indexOf( " ", i + 1 );
            res.x4 = normalize_int( str.substring( i + 1, str.length ), 10 );
            i = str.indexOf( " ", i + 1 );
            res.x1 = normalize_int( str.substring( i + 1, str.length ), 10 );
        }
        return res;
    }
    return null;
}


function viso_clip( vis_obj, clip_area ) {
    if( !vis_obj ) return;

    if( ns4 ) {
        vis_obj.clip.left   = clip_area.x1;
        vis_obj.clip.top    = clip_area.x2;
        vis_obj.clip.right  = clip_area.x3;
        vis_obj.clip.bottom = clip_area.x4;
    }
    else if( ie || dom )
        vis_obj.style.clip = 'rect( ' + clip_area.x2 + ' ' 
                                      + clip_area.x3 + ' ' 
                                      + clip_area.x4 + ' '
                                      + clip_area.x1 + ' )';
}

function viso_change_clip_by( vis_obj, clip_diff ) {
    if( !vis_obj ) return;

    var clip_area = viso_get_clip_area( vis_obj );

    viso_clip( vis_obj, clip_area.add( clip_diff ) );
}

function viso_get_clip_width( vis_obj ) {
    if( ns4 )
        return normalize_int( vis_obj.clip.width );
    if( ie || dom ) {
        if( !vis_obj.style.clip )
            return normalize_int( vis_obj.style.width );
        var clip_area = viso_get_clip_area( vis_obj );

        return clip_area.x3 - clip_area.x1;
    }
    return -1;
}

function viso_get_clip_height( vis_obj ) {
    if( ns4 )
        return normalize_int( vis_obj.clip.height );
    if( ie || dom ) {
        if( !vis_obj.style.clip )
            return normalize_int( vis_obj.style.height );
        var clip_area = viso_get_clip_area( vis_obj );

        return clip_area.x4 - clip_area.x2;
    }
    return -1;
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

    dx = normalize_int( dx );
    dy = normalize_int( dy );

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
    viso_clip( vis_obj, clip_area.add( new Quadruple( dx, dy, dx, dy ) ) );
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

    var new_pos = orig_position.add( shift );

    return viso_scroll_to( vis_obj, new_pos.x, new_pos.y, true );
}

//-----------------------------------------------------------------------------
// object background.
//-----------------------------------------------------------------------------

function viso_set_bg_color( vis_obj, color ) {
    if( !vis_obj ) return;

    if( ns4 )
        vis_obj.bgColor = color;
    else if( ie || dom )
        vis_obj.style.backgroundColor = color;
}

function viso_set_bg_image( vis_obj, src ) {
    if( !vis_obj ) return;

    if( ns4 )
        vis_obj.background.src = src;
    else if( ie || dom )
        vis_obj.style.backgroundImage = "url( " + src + " )";
}

//-----------------------------------------------------------------------------
// events.handling.
//-----------------------------------------------------------------------------

function viso_add_event_handler( event_id, handler, vis_obj, in_front ) {
    if( !handler.is_a( Callback ) )
        handler = new Callback( handler );

    if( ie ) {
        vis_obj = vis_obj ? vis_obj : document.body;

        var curr_handler = vis_obj[event_id];

        if( !curr_handler ) {
            vis_obj[event_id] = function() {
                handler.run( window.event );
            };

            return;
        }

        if( in_front ) {
            vis_obj[event_id] = function() {
                handler.run( window.event );
                curr_handler( window.event );
            };
        }
        else {
            vis_obj[event_id] = function() {
                curr_handler( window.event );
                handler.run( window.event );
            };
        }
    }
    else if( dom ) {
        vis_obj = vis_obj ? vis_obj : window;

        var curr_handler = vis_obj[event_id];

        if( !curr_handler ) {
            vis_obj[event_id] = function( e ) {
                handler.run( e );
            };
            return;
        }

        if( in_front ) {
            vis_obj[event_id] = function( e ) {
                handler.run( e );
                curr_handler( e );
            };
        }
        else {
            vis_obj[event_id] = function() {
                curr_handler( e );
                handler.run( e );
            };
        }
    }
}

function viso_clean_event_handlers( event_id, vis_obj ) {
    if( !vis_obj )
        vis_obj = ie ? document.body : window;

    delete vis_obj[event_id];
}

function viso_event_offset_x( e ) {
    if( ie )
        return e.offsetX;
    if( dom )
        return e.layerX;

    return null;
}

function viso_event_offset_y( e ) {
    if( ie )
        return e.offsetY;
    if( dom )
        return e.layerY;

    return null;
}

function viso_event_target( e ) {
    if( ie )
        return e.srcElement;
    if( dom )
        return e.target;

    return null;
}

function viso_event_to_target( e ) {
    if( ie )
        return e.toElement;
    if( dom )
        return e.relatedTarget;

    return null;
}

// EOF
