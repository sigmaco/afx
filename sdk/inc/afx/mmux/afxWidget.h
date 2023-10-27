////////////////////////////////////////////////////////////////////////////////
//                     The Standard QWADRO Implementation                     //
////////////////////////////////////////////////////////////////////////////////

#ifndef AFX_WIDGET_H
#define AFX_WIDGET_H

#include "afxMmuiDefs.h"
#include "afx/draw/afxColor.h"
#include "afx/core/afxChain.h"
#include "afx/core/afxInstance.h"
#include "afx/core/afxUri.h"
#include "afx/math/afxTransform.h"
//#include "afxViewport.h"
//#include "afx/draw/afxDrawInput.h"

typedef enum afxWidgetType
{
    //NIL
    AFX_WIDG_PANEL          = AfxPackFcc('w', 'p', 'a', 'n'),
    AFX_WIDG_LABEL          = AfxPackFcc('w', 'l', 'a', 'b'),
    AFX_WIDG_BUTTON         = AfxPackFcc('w', 'b', 'u', 't'),
    AFX_WIDG_CHECKBOX       = AfxPackFcc('w', 'c', 'h', 'k'),
    AFX_WIDG_EDITBOX        = AfxPackFcc('w', 'e', 'd', 't'),
    AFX_WIDG_SLIDER         = AfxPackFcc('w', 's', 'l', 'd'),
    AFX_WIDG_DROPDOWN       = AfxPackFcc('w', 'd', 'r', 'p'),
    AFX_WIDG_PROGRESSBAR    = AfxPackFcc('w', 'p', 'r', 'o'),
    AFX_WIDG_PICTURE        = AfxPackFcc('w', 'p', 'i', 'c'),
    AFX_WIDG_VIDEO          = AfxPackFcc('w', 'v', 'i', 'd'),
    AFX_WIDG_GROUP          = AfxPackFcc('w', 'g', 'r', 'p'),

    AFX_WIDG_TYPE_TOTAL
} afxWidgetType;

AFX_DEFINE_STRUCT( afxWidgetStyle)
{
    afxRect     rect;
};

// screen overlay is a draw input device that has been idealized to be an widget layer.

AFX_DEFINE_STRUCT( afxWidgetVertex)
{
    afxV2d      xy, uv;
};

AFX_DECLARE_STRUCT(afxWidgetImplementation);
AFX_DECLARE_STRUCT(afxWidgetImplementationData);

AFX_DEFINE_STRUCT(afxWidgetConfig)
{
    afxString const *name;
    afxWidget parent;
    afxUri const *uri;
    afxResult(*f)(afxWidget, afxUri const*, void *data);
};

#ifdef _AFX_WIDGET_C
AFX_OBJECT(afxWidget)
{
    afxLinkage                      parent;
    afxChain                        children;

    afxTransform                    local;
    afxM4d                          iw;

    afxResult                       (*update)(afxWidget wid, afxReal dt);

    afxWidgetImplementation const   *impl;
    afxWidgetImplementationData     *data;

    // common
    afxV2d                          anchor; // -1 ... 0 ... 1, left-center-right, top-middle-bottom
    
    afxString*                      text; // 2048
    afxString*                      hint; // 2048
    afxByte                         extra[512];
    struct
    {
        afxConnection               bgTexture;
        afxColor                    color, bgColor;

        afxNat                      visibility;
        afxNat                      cursor; // 'arrow', 'pointer' for links

        // padding, marging
        // border length, border radius

        afxReal                     lineHeight;
        afxReal                     letterSpacing;
        afxNat                      wordBreak;

        struct
        {
            afxString*              face; // 32 // "sans"
            afxReal                 size; // 18.0 (px)
            afxNat                  style; // normal
            afxNat                  weight; // 400
        }                           font;

    };
};
#endif

struct afxWidgetImplementation
{
    afxWidgetType   fcc;
    afxChar const   *name;
    afxResult       (*dtor)(afxWidget);
    void            (*click)(afxWidget, afxV2d);
};

AFX void        AfxWidgetReanchor(afxWidget wid, afxV2d const point);

AFX void        AfxWidgetAddTranslation(afxWidget wid, afxV4d const translation);
AFX void        AfxWidgetSetTranslation(afxWidget wid, afxV4d const translation);

AFX void        AfxWidgetAddRotation(afxWidget wid, afxQuat const rotation);
AFX void        AfxWidgetSetRotation(afxWidget wid, afxQuat const rotation);

AFX void        AfxWidgetAddMakeScale(afxWidget wid, afxV3d const scale);
AFX void        AfxWidgetSetScale(afxWidget wid, afxV3d const scale);

AFX afxResult   AfxWidgetExecuteQuery(afxWidget wid, afxUri const *uri);

#endif//AFX_WIDGET_H

#if 0

gtk_widget_action_set_enabled
Enable or disable an action installed with gtk_widget_class_install_action().

gtk_widget_activate
For widgets that can be “activated”(buttons, menu items, etc.), this function activates them.

gtk_widget_activate_action
Looks up the action in the action groups associated with widget and its ancestors, and activates it.

gtk_widget_activate_action_variant
Looks up the action in the action groups associated with widget and its ancestors, and activates it.

gtk_widget_activate_default
Activates the default.activate action from widget.

gtk_widget_add_controller
Adds controller to widget so that it will receive events.

gtk_widget_add_css_class
Adds a style class to widget.

gtk_widget_add_mnemonic_label
Adds a widget to the list of mnemonic labels for this widget.

gtk_widget_add_tick_callback
Queues an animation frame update and adds a callback to be called before each frame.

gtk_widget_allocate
This function is only used by GtkWidget subclasses, to assign a size, position and (optionally)baseline to their child widgets.

gtk_widget_child_focus
Called by widgets as the user moves around the window using keyboard shortcuts.

gtk_widget_compute_bounds
Computes the bounds for widget in the coordinate space of target.

gtk_widget_compute_expand
Computes whether a container should give this widget extra space when possible.

gtk_widget_compute_point
Translates the given point in widget‘s coordinates to coordinates relative to target’s coordinate system.

gtk_widget_compute_transform
Computes a matrix suitable to describe a transformation from widget‘s coordinate system into target‘s coordinate system.

gtk_widget_contains
Tests if the point at(x, y) is contained in widget.

gtk_widget_create_pango_context
Creates a new PangoContext with the appropriate font map, font options, font description, and base direction for drawing text for this widget.

gtk_widget_create_pango_layout
Creates a new PangoLayout with the appropriate font map, font description, and base direction for drawing text for this widget.

gtk_drag_check_threshold
Checks to see if a drag movement has passed the GTK drag threshold.

gtk_widget_error_bell
Notifies the user about an input - related error on this widget.

gtk_widget_get_allocated_baseline
Returns the baseline that has currently been allocated to widget.

gtk_widget_get_allocated_height
Returns the height that has currently been allocated to widget.

gtk_widget_get_allocated_width
Returns the width that has currently been allocated to widget.

gtk_widget_get_allocation
Retrieves the widget’s allocation.

gtk_widget_get_ancestor
Gets the first ancestor of widget with type widget_type.

gtk_widget_get_can_focus
Determines whether the input focus canv enter widget or any of its children.

gtk_widget_get_can_target
Queries whether widget canv be the target of pointer events.

gtk_widget_get_child_visible
Gets the value set with gtk_widget_set_child_visible().

gtk_widget_get_clipboard
Gets the clipboard object for widget.

gtk_widget_get_css_classes
Returns the list of style classes applied to widget.

gtk_widget_get_css_name
Returns the CSS name that is used for self.

gtk_widget_get_cursor
Queries the cursor set on widget.

gtk_widget_get_direction
Gets the reading direction for a particular widget.

gtk_widget_get_display
Get the GdkDisplay for the toplevel window associated with this widget.

gtk_widget_get_first_child
Returns the widgets first child.

gtk_widget_get_focus_child
Returns the current focus child of widget.

gtk_widget_get_focus_on_click
Returns whether the widget should grab focus when it is clicked with the mouse.

gtk_widget_get_focusable
Determines whether widget canv own the input focus.

gtk_widget_get_font_map
Gets the font map of widget.

gtk_widget_get_font_options
Returns the cairo_font_options_t of widget.

gtk_widget_get_frame_clock
Obtains the frame clock for a widget.

gtk_widget_get_halign
Gets the horizontal alignment of widget.

gtk_widget_get_has_tooltip
Returns the current value of the has - tooltip property.

gtk_widget_get_height
Returns the content height of the widget.

gtk_widget_get_hexpand
Gets whether the widget would like any available extra horizontal space.

gtk_widget_get_hexpand_set
Gets whether gtk_widget_set_hexpand() has been used to explicitly set the expand flag on this widget.

gtk_widget_get_last_child
Returns the widgets last child.

gtk_widget_get_layout_manager
Retrieves the layout manager used by widget.

gtk_widget_get_mapped
Whether the widget is mapped.

gtk_widget_get_margin_bottom
Gets the bottom margin of widget.

gtk_widget_get_margin_end
Gets the end margin of widget.

gtk_widget_get_margin_start
Gets the start margin of widget.

gtk_widget_get_margin_top
Gets the top margin of widget.

gtk_widget_get_name
Retrieves the name of a widget.

gtk_widget_get_native
Returns the nearest GtkNative ancestor of widget.

gtk_widget_get_next_sibling
Returns the widgets next sibling.

gtk_widget_get_opacity
Fetches the requested opacity for this widget.

gtk_widget_get_overflow
Returns the widgets overflow value.

gtk_widget_get_pango_context
Gets a PangoContext with the appropriate font map, font description, and base direction for this widget.

gtk_widget_get_parent
Returns the parent widget of widget.

gtk_widget_get_preferred_size
Retrieves the minimum and natural size of a widget, taking into account the widget’s preference for height - for - width management.

gtk_widget_get_prev_sibling
Returns the widgets previous sibling.

gtk_widget_get_primary_clipboard
Gets the primary clipboard of widget.

gtk_widget_get_realized
Determines whether widget is realized.

gtk_widget_get_receives_default
Determines whether widget is always treated as the default widget within its toplevel when it has the focus, even if another widget is the default.

gtk_widget_get_request_mode
Gets whether the widget prefers a height - for - width layout or a width - for - height layout.

gtk_widget_get_root
Returns the GtkRoot widget of widget.

gtk_widget_get_scale_factor
Retrieves the internal scale factor that maps from window coordinates to the actual device pixels.

gtk_widget_get_sensitive
Returns the widget’s sensitivity.

gtk_widget_get_settings
Gets the settings object holding the settings used for this widget.

gtk_widget_get_size
Returns the content width or height of the widget.

gtk_widget_get_size_request
Gets the size request that was explicitly set for the widget using gtk_widget_set_size_request().

gtk_widget_get_state_flags
Returns the widget state as a flag set.

gtk_widget_get_style_context
Returns the style context associated to widget.

gtk_widget_get_template_child
Fetch an object build from the template XML for widget_type in this widget instance.

gtk_widget_get_tooltip_markup
Gets the contents of the tooltip for widget.

gtk_widget_get_tooltip_text
Gets the contents of the tooltip for widget.

gtk_widget_get_valign
Gets the vertical alignment of widget.

gtk_widget_get_vexpand
Gets whether the widget would like any available extra vertical space.

gtk_widget_get_vexpand_set
Gets whether gtk_widget_set_vexpand() has been used to explicitly set the expand flag on this widget.

gtk_widget_get_visible
Determines whether the widget is visible.

gtk_widget_get_width
Returns the content width of the widget.

gtk_widget_grab_focus
Causes widget to have the keyboard focus for the GtkWindow it’s inside.

gtk_widget_has_css_class
Returns whether css_class is currently applied to widget.

gtk_widget_has_default
Determines whether widget is the current default widget within its toplevel.

gtk_widget_has_focus
Determines if the widget has the global input focus.

gtk_widget_has_visible_focus
Determines if the widget should show a visible indication that it has the global input focus.

gtk_widget_hide
Reverses the effects of gtk_widget_show().

gtk_widget_in_destruction
Returns whether the widget is currently being destroyed.

gtk_widget_init_template
Creates and initializes child widgets defined in templates.

gtk_widget_insert_action_group
Inserts group into widget.

gtk_widget_insert_after
Inserts widget into the child widget list of parent.

gtk_widget_insert_before
Inserts widget into the child widget list of parent.

gtk_widget_is_ancestor
Determines whether widget is somewhere inside ancestor, possibly with intermediate containers.

gtk_widget_is_drawable
Determines whether widget canv be drawn to.

gtk_widget_is_focus
Determines if the widget is the focus widget within its toplevel.

gtk_widget_is_sensitive
Returns the widget’s effective sensitivity.

gtk_widget_is_visible
Determines whether the widget and all its parents are marked as visible.

gtk_widget_keynav_failed
Emits the::keynav - failed signal on the widget.

gtk_widget_list_mnemonic_labels
Returns the widgets for which this widget is the target of a mnemonic.

gtk_widget_map
Causes a widget to be mapped if it isn’t already.

gtk_widget_measure
Measures widget in the orientation orientation and for the given for_size.

gtk_widget_mnemonic_activate
Emits the::mnemonic - activate signal.

gtk_widget_observe_children
Returns a GListModel to track the children of widget.

gtk_widget_observe_controllers
Returns a GListModel to track the GtkEventControllers of widget.

gtk_widget_pick
Finds the descendant of widget closest to the point(x, y).

gtk_widget_queue_allocate
Flags the widget for a rerun of the Gtk.WidgetClass.size_allocate function.

gtk_widget_queue_draw
Schedules this widget to be redrawn in the paint phase of the current or the next frame.

gtk_widget_queue_resize
Flags a widget to have its size renegotiated.

gtk_widget_realize
Creates the GDK resources associated with a widget.

gtk_widget_remove_controller
Removes controller from widget, so that it doesn’t process events anymore.

gtk_widget_remove_css_class
Removes a style from widget.

gtk_widget_remove_mnemonic_label
Removes a widget from the list of mnemonic labels for this widget.

gtk_widget_remove_tick_callback
Removes a tick callback previously registered with gtk_widget_add_tick_callback().

gtk_widget_set_can_focus
Specifies whether the input focus canv enter the widget or any of its children.

gtk_widget_set_can_target
Sets whether widget canv be the target of pointer events.

gtk_widget_set_child_visible
Sets whether widget should be mapped along with its parent.

gtk_widget_set_css_classes
Clear all style classes applied to widget and replace them with classes.

gtk_widget_set_cursor
Sets the cursor to be shown when pointer devices point towards widget.

gtk_widget_set_cursor_from_name
Sets a named cursor to be shown when pointer devices point towards widget.

gtk_widget_set_direction
Sets the reading direction on a particular widget.

gtk_widget_set_focus_child
Set child as the current focus child of widget.

gtk_widget_set_focus_on_click
Sets whether the widget should grab focus when it is clicked with the mouse.

gtk_widget_set_focusable
Specifies whether widget canv own the input focus.

gtk_widget_set_font_map
Sets the font map to use for Pango rendering.

gtk_widget_set_font_options
Sets the cairo_font_options_t used for Pango rendering in this widget.

gtk_widget_set_halign
Sets the horizontal alignment of widget.

gtk_widget_set_has_tooltip
Sets the has - tooltip property on widget to has_tooltip.

gtk_widget_set_hexpand
Sets whether the widget would like any available extra horizontal space.

gtk_widget_set_hexpand_set
Sets whether the hexpand flag will be used.

gtk_widget_set_layout_manager
Sets the layout manager delegate instance that provides an implementation for measuring and allocating the children of widget.

gtk_widget_set_margin_bottom
Sets the bottom margin of widget.

gtk_widget_set_margin_end
Sets the end margin of widget.

gtk_widget_set_margin_start
Sets the start margin of widget.

gtk_widget_set_margin_top
Sets the top margin of widget.

gtk_widget_set_name
Sets a widgets name.

gtk_widget_set_opacity
Request the widget to be rendered partially transparent.

gtk_widget_set_overflow
Sets how widget treats content that is drawn outside the widget’s content area.

gtk_widget_set_parent
Sets parent as the parent widget of widget.

gtk_widget_set_receives_default
Specifies whether widget will be treated as the default widget within its toplevel when it has the focus, even if another widget is the default.

gtk_widget_set_sensitive
Sets the sensitivity of a widget.

gtk_widget_set_size_request
Sets the minimum size of a widget.

gtk_widget_set_state_flags
Turns on flag values in the current widget state.

gtk_widget_set_tooltip_markup
Sets markup as the contents of the tooltip, which is marked up with Pango markup.

gtk_widget_set_tooltip_text
Sets text as the contents of the tooltip.

gtk_widget_set_valign
Sets the vertical alignment of widget.

gtk_widget_set_vexpand
Sets whether the widget would like any available extra vertical space.

gtk_widget_set_vexpand_set
Sets whether the vexpand flag will be used.

gtk_widget_set_visible
Sets the visibility state of widget.

gtk_widget_should_layout
Returns whether widget should contribute to the measuring and allocation of its parent.

gtk_widget_show
Flags a widget to be displayed.

gtk_widget_size_allocate
Allocates widget with a transformation that translates the origin to the position in allocation.

gtk_widget_snapshot_child
Snapshot the a child of widget.

gtk_widget_translate_coordinates
Translate coordinates relative to src_widget’s allocation to coordinates relative to dest_widget’s allocations.

gtk_widget_trigger_tooltip_query
Triggers a tooltip query on the display where the toplevel of widget is located.

gtk_widget_unmap
Causes a widget to be unmapped if it’s currently mapped.

gtk_widget_unparent
Dissociate widget from its parent.

gtk_widget_unrealize
Causes a widget to be unrealized(frees all GDK resources associated with the widget).

gtk_widget_unset_state_flags
Turns off flag values for the current widget state.

#endif