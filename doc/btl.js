JS.include( "vis_object_api.js", viso_path );
JS.include( "wipe_away_menu.js", viso_path );
JS.include( "cookies.js", viso_path );

function get_copyright_date() {
   document.write( "2001-2002" )
}

function contact_addess() {
   document.write( "<a href='mailto:rogeeff@emailaccount.com'>Gennadiy Rozental</a>" )
}

var styles = [ "btl", "btl-simple" ];
function put_screen_style() {
    var btl_style       = new Cookie( "btl_style" );

    if( !btl_style.load() )
        btl_style.v = 0;

    var str = '<link id="screen_style" rel="stylesheet" type="text/css" href="style/' + 
              styles[btl_style.v] + '.css" media="screen" />';

    document.write( str );
}

function style_switch() { 
    var btl_style       = new Cookie( "btl_style" );

    if( !btl_style.load() )
        btl_style.v = 0;

    btl_style.v++;
    btl_style.v %= styles.length;

    btl_style.store( { days:365 } );

    window.location.reload( false );
}
    
function btl_header() {
    document.write( "<div class='print-header'>" );
    document.write( "<img border='0' src='imgs/c++boost_bw.gif' style='padding-top:0; margin-top:0; '>" );
    document.write( "</div>" );

    document.write( "<div class='screen-header'>" );
    document.write( "<table width='100%' style='border-style: none; padding:0;' cellspacing=0; id='active_area'>" );
    document.write( "<tr>" );

    document.write( "<td align='left'>" );
    document.write( "<a name='TOP'><img border='0' src='../../../c++boost.gif' width='277' height='86'></a><br clear=left>" );
    document.write( "</td>" );

    document.write( "<td align='right' width='70%'>" );

    document.write( "<div id='wipe-away-menu-holder'>" );

    var btl_style       = new Cookie( "btl_style" );

    if( !btl_style.load() )
        btl_style.v = 0;

    document.write( "<div id='header_menu' class='header-menu'><nobr>" );
    document.write( "<a href='index.htm'>Home</a> " );
    document.write( "<a href='faq.htm'>FAQ</a> " );
    document.write( "<a href='../../../index.htm'>Boost home</a> " );
    document.write( "<a href='../../../libs/libraries.htm'>Boost libraries</a> " );
    document.write( "</nobr></div>" );

    if( btl_style.v == 0 )
        document.write( "<img id='header_menu_root' class='header-menu-root' src='imgs/menu_root.gif' onMouseOver='this.src=\"imgs/menu_root_on.gif\";btl_menu.show();' onMouseOut='this.src=\"imgs/menu_root.gif\";' >" );
    else
        document.write( "<img id='header_menu_root' class='header-menu-root' src='imgs/menu_root.gif' onMouseOver='btl_menu.show();'>" );

    document.write( "</div>" );

    document.write( "</td></tr></table></div>" );
}

function hide_btl_menu( e ) {
    if( viso_event_to_target( e ) == viso_get("body") )
        btl_menu.hide();
}

function btl_menu_init() {
    btl_menu = new WipeAwayMenu( "header_menu", "header_menu_root", true, "left" );
    viso_add_event_handler( "mouseout", hide_btl_menu, viso_get( "active_area" ) )
    viso_add_event_handler( "click", style_switch, viso_get( "header_menu_root" ) )   
}

function put_ref_to_top() {
    document.write( "<span class='ref-to-top'>" );
    var str = "<a href='#TOP'>" +
      "<img border='0' src='imgs/uarrow.gif' align='right' width='30'" +
      "onMouseDown='this.src=\"imgs/uarrow_pressed.gif\"; return true;'; " + 
      "onMouseOut='this.src=\"imgs/uarrow.gif\"; window.status=window.defaultStatus; return true;' " +
      "onMouseOver='window.status=\"Jump to beggining\"; return true;' ></a>";
    document.write( str );

    document.write( "</span>" );
}


function put_examples_roller( roller_name ) {
    document.write( "<span class='example_roller'>" );

    document.write( 
        "<a href='javascript: void " + roller_name + ".roll();'" +
        "onMouseOver='window.status=\"Next example\"; return true;'" +
        "onMouseOut='window.status=window.defaultStatus; return true;' >" +
        "<img src='imgs/next_arrow.gif' style='border-style:none;' align='top' height='22' /></a>" );

    document.write( "</span>" );
}

