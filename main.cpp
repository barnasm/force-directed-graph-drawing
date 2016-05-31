/*
Michał Barnaś
Uniwersytet Wrocławski
Programowanie Obiektowe 2016
*/

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <cstring>
#include <memory>
#include "forcedirectedgraphdrawing.h"
#include <thread>

using MDtype = double;

double EDGE_WIDTH = 3;
double RADIUS = 30;

#define MAX_VERTICES 1000
#define MAX_EDGES 1000
#define MAX_RADIUS 100
#define MAX_EDGE_WIDTH 100

int HEIGHT = 600;
int WIDTH = 800;
MDtype MAX_DISPLACEMENT;


int VERTEX_GEN = 20;
int EDGE_GEN   = 20;
MDtype GRAVITY    = 0.05;
MDtype ATTRACTION = 1.0;
MDtype REPULSION  = 1.0;
MDtype SPEED      = 0.01;
int JUMP = 1;
int TIMER_SLEEP = 30;
int timerId;

static cairo_surface_t *layerBackground;
static cairo_surface_t *layerEdges;
static cairo_surface_t *layerVertices;
static cairo_surface_t *layerMessage;

#define NameSpinVertexRadius     "NameSpinVertexRadius"
#define NameSpinEdgeWidth        "NameSpinEdgeWidth"

#define NameSpinNumOfVertices    "NameSpinNumOfVertices"
#define NameSpinNumOfEdge        "NameSpinNumOfEdge"
#define NameTextBoxGravity       "NameTextBoxGravity"
#define NameTextBoxAttraction    "NameTextBoxAttrection"
#define NameTextBoxRepulsion     "NameTextBoxRepulsion"
#define NameTextBoxSpeed         "NameTextBoxSpeed"
#define NameSpinJump             "NameSpinJump"
#define NameSpinTimerSleep       "NameSpinTimerSleep"
#define NameButtonHowToUse       "NameButtonHowToUse"
#define NameButtonStopAnimation  "NameButtonStopAnimation"
#define NameButtonGenNewGraph    "NameButtonGenNewGraph"

static gboolean continue_timer = FALSE;
static gboolean start_timer = FALSE;

GtkWidget *drawing_area;

static void activate();
static gboolean timeOut(gpointer data);

std::shared_ptr<GraphClass> myGraph;
bool MOVE_THIS_VERTEX = TRUE;

int main (int argc, char **argv)
{
    //GraphClass graph(&HEIGHT, &WIDTH, &VERTEX_GEN, &EDGE_GEN, &RADIUS);
    //graph.generateGraph();
    //GraphClass graph2(graph);
    //GraphClass graph2 = graph;
    //graph = graph3;
    //std::cout << graph.vertices << "  " << graph2.vertices << std::endl;

    srand(time(nullptr));
    myGraph = std::make_shared<GraphClass> (&HEIGHT, &WIDTH, &VERTEX_GEN, &EDGE_GEN, &RADIUS);
    //myGraph = std::make_shared<GraphClass> (graph);
    myGraph->generateGraph();
    //std::cout << *myGraph;

    gtk_init(&argc, &argv);

    HEIGHT = gdk_screen_get_height(gdk_screen_get_default()) - 100;
    WIDTH  = gdk_screen_get_width(gdk_screen_get_default())  - 400;
    MAX_DISPLACEMENT = sqrt(WIDTH*HEIGHT);

    activate();
    gtk_main();

    return 0;
}



static void drawEdge (GtkWidget *widget, std::shared_ptr <EdgeClass> e){
    cairo_t *cr;
    cr = cairo_create (layerEdges);

    cairo_set_line_width (cr, EDGE_WIDTH);
    //cairo_set_source_rgba (cr, 255.0/255.0, 67.0/255.0, 0.0/255.0, 1);
    cairo_set_source_rgba (cr, e->color->getRed()   -0.2,
                               e->color->getGreen() -0.2,
                               e->color->getBlue()  -0.2, 1);
    cairo_set_line_cap  (cr, CAIRO_LINE_CAP_ROUND);
    cairo_move_to (cr, e->A->x, e->A->y);
    cairo_line_to (cr, e->B->x, e->B->y);
    cairo_stroke (cr);
    cairo_destroy (cr);
}
static void drawVertex (GtkWidget *widget, std::shared_ptr <VertexClass> v )
{
    cairo_t *cr;
    cairo_pattern_t *pat;
    cr = cairo_create (layerVertices);

    pat = cairo_pattern_create_radial (v->x, v->y, RADIUS/100.0,
                                       v->x, v->y, RADIUS);
    cairo_pattern_add_color_stop_rgba (pat, 0, (double)v->color->getRed(),
                                               (double)v->color->getGreen(),
                                               (double)v->color->getBlue(), 1);
    cairo_pattern_add_color_stop_rgba (pat, 1, 0, 0, 0, 1);
    cairo_set_source (cr, pat);
    //cairo_arc (cr, 128, 128, 76, 0, 2 * G_PI);
    cairo_arc (cr, v->x, v->y, RADIUS/2.0, 0, 2*G_PI);
    cairo_fill (cr);
    cairo_pattern_destroy (pat);

    cairo_destroy (cr);

    gtk_widget_queue_draw_area (widget, v->x-RADIUS, v->y-RADIUS, RADIUS*1.6, RADIUS*1.6);
}
static void clearSurface ()
{
    cairo_t *cr;

    if(continue_timer){
        cr = cairo_create (layerMessage);
        cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
        cairo_paint (cr);
        cairo_destroy (cr);
    }

    cr = cairo_create (layerEdges);
    cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint (cr);
    cairo_destroy (cr);

    cr = cairo_create (layerVertices);
    cairo_set_operator (cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint (cr);
    cairo_destroy (cr);

    cr = cairo_create (layerBackground);
    cairo_pattern_t *pat = cairo_pattern_create_linear (0.0, 0.0,  0.0, WIDTH);
    cairo_pattern_add_color_stop_rgba (pat, 1, 250.0/255.0, 229.0/255.0, 255.0/255.0, 1);
    cairo_pattern_add_color_stop_rgba (pat, 0, 237.0/255.0, 236.0/255.0, 255.0/255.0, 1);
    cairo_set_source (cr, pat);
    cairo_paint (cr);
    cairo_destroy (cr);
    cairo_pattern_destroy (pat);
}

void computeGraphPos(std::shared_ptr<GraphClass> g){
    if(g == nullptr) return;

    for(int i = 0; i < JUMP; ++i)
    {
        ForceDirectedGraphDrawing FDGD_Alg(&HEIGHT, &WIDTH, &GRAVITY, &REPULSION, &ATTRACTION, &SPEED, &MAX_DISPLACEMENT);
        FDGD_Alg.computeIteration(g, MOVE_THIS_VERTEX);
    }
}
void drawGraph(std::shared_ptr<GraphClass>g){
    if(g == nullptr) return;
    //clearSurface();
    for(unsigned int i = 0; i < g->vertices->size(); i++){
        //myGraph->thisVertex = g->graphV[i];
        drawVertex(drawing_area, g->vertices->at(i));
    }
    for(unsigned int i = 0; i < g->edges->size(); i++){
        //myGraph->thisEdge = g->graphE[i];
        drawEdge(drawing_area, g->edges->at(i));
    }
}
void preDrawGraph(GtkButton *button, GtkWidget* widget){
    clearSurface ();
    gtk_widget_queue_draw(widget);
    if(continue_timer || button)
    {
      	computeGraphPos(myGraph);
    }
    //    std::thread th1(&computeGraphPos, std::ref(myGraph));

    drawGraph(myGraph);
    //    th1.join();
}

static void startTimer ()
{
    if(!start_timer)
    {
        timerId = g_timeout_add(TIMER_SLEEP, timeOut, nullptr);
        start_timer = TRUE;
        continue_timer = TRUE;
    }
}
static gboolean timeOut(gpointer data)
{
    if(!start_timer){
        startTimer();
        return FALSE;
    }

    preDrawGraph(GTK_BUTTON(data), drawing_area);
    return continue_timer;
}
static void onOffTimer (bool on)
{
    if(start_timer)
    {
        continue_timer = on;
        if(continue_timer)
        {
            timerId = g_timeout_add(TIMER_SLEEP, timeOut, nullptr);
        }
    }
}

static void drawHowToUse (GtkWidget *widget)
{
    GdkPixbuf *pix;
    GError *err = nullptr;
    /* Create pixbuf */
    pix = gdk_pixbuf_new_from_file("moUse.png", &err);
    if(err)
    {
        printf("Error : %s\n", err->message);
        GtkWidget *dialog = gtk_message_dialog_new (nullptr,
                                         GTK_DIALOG_USE_HEADER_BAR,
                                         GTK_MESSAGE_ERROR,
                                         GTK_BUTTONS_CLOSE,
                                         "Error: “%s”",
                                         err->message);
        gtk_dialog_run (GTK_DIALOG (dialog));
        gtk_widget_destroy (dialog);

        g_error_free(err);
        return;
    }
    onOffTimer(!continue_timer);
    cairo_t *cr;
    cr = cairo_create (layerMessage);
    gdk_cairo_set_source_pixbuf(cr, pix, 100, 0);
    cairo_paint(cr);
    cairo_destroy (cr);
    gtk_widget_queue_draw_area (widget, 100, 0, 1000, 300);
}

void buttonClicked(GtkWidget *button, gpointer widget){
    if(strcmp(gtk_widget_get_name(button), NameButtonHowToUse) == 0){
        drawHowToUse(drawing_area);
        return;
    }
    if(strcmp(gtk_widget_get_name(button), NameButtonStopAnimation) == 0){
        if( strcmp(gtk_button_get_label(GTK_BUTTON(button)), "Start") == 0){
            gtk_button_set_label(GTK_BUTTON(button), "Stop");
            onOffTimer(true);
        }
        else{
            gtk_button_set_label(GTK_BUTTON(button), "Start");
            onOffTimer(false);
        }

        return;
    }
    clearSurface ();
    gtk_widget_queue_draw((GtkWidget*)widget);


    if(strcmp(gtk_widget_get_name(button), NameButtonGenNewGraph) == 0) {
        myGraph->generateGraph();
        //myGraph->thisVertex = myGraph->vertices->at(0);
        //myGraph->thisEdge = myGraph->edges->at(0);
        //printGraphData(myGraph);
    }
    drawGraph(myGraph);
}
void dataWidgetChanged(GtkWidget *widget, gpointer object){
    //printf("data changed\n");
    const char *str = gtk_widget_get_name((GtkWidget*)object);

//left menu
    if(strcmp(str, NameSpinVertexRadius) == 0){
        RADIUS = (int)floor(gtk_adjustment_get_value (gtk_spin_button_get_adjustment ((GtkSpinButton*)object)));
        preDrawGraph(nullptr, drawing_area);
        return;
    }else if(strcmp(str, NameSpinEdgeWidth) == 0){
        EDGE_WIDTH = (int)floor(gtk_adjustment_get_value (gtk_spin_button_get_adjustment ((GtkSpinButton*)object)));
        preDrawGraph(nullptr, drawing_area);
        return;
    }else if(strcmp(str, NameSpinTimerSleep) == 0){
        //g_source_remove(timerId);
        start_timer = false;
        TIMER_SLEEP = (int)floor(gtk_adjustment_get_value (gtk_spin_button_get_adjustment ((GtkSpinButton*)object)));
        return;
    }
//right menu
    else if(strcmp(str, NameSpinNumOfVertices) == 0){
        VERTEX_GEN = (int)floor(gtk_adjustment_get_value (gtk_spin_button_get_adjustment ((GtkSpinButton*)object)));
        return;
    }else if(strcmp(str, NameSpinNumOfEdge) == 0){
        EDGE_GEN = (int)floor(gtk_adjustment_get_value (gtk_spin_button_get_adjustment ((GtkSpinButton*)object)));
        return;
    }else if(strcmp(str, NameTextBoxGravity) == 0){
        sscanf(gtk_entry_get_text (GTK_ENTRY (object)), "%lf", &GRAVITY);
        return;
    }else if(strcmp(str, NameTextBoxAttraction) == 0){
        sscanf(gtk_entry_get_text (GTK_ENTRY (object)), "%lf", &ATTRACTION);
        return;
    }else if(strcmp(str, NameTextBoxRepulsion) == 0){
        sscanf(gtk_entry_get_text (GTK_ENTRY (object)), "%lf", &REPULSION);
        return;
    }else if(strcmp(str, NameTextBoxSpeed) == 0){
        sscanf(gtk_entry_get_text (GTK_ENTRY (object)), "%lf", &SPEED);
        return;
    }else if(strcmp(str, NameSpinJump) == 0){
        JUMP = (int)floor(gtk_adjustment_get_value (gtk_spin_button_get_adjustment ((GtkSpinButton*)object)));
        return;
    }
}


template<typename T>
GtkWidget* createSpinButton(const char *name, int min, int max, GtkWidget *boxMenu/*, GtkWidget *spin*/, T n, const char *oName){
    GtkAdjustment *adjustment;

    adjustment = gtk_adjustment_new (n, min, max, 1, 10, 0);

    GtkWidget* spin = gtk_spin_button_new(adjustment, 1, 0);
    gtk_widget_set_name(spin, oName);

    GtkWidget *frameText = gtk_frame_new (name);
    gtk_container_add (GTK_CONTAINER (boxMenu), frameText);
    gtk_container_add (GTK_CONTAINER (frameText), spin);
    //gtk_box_pack_start(GTK_BOX(boxMenuR), spin, TRUE, TRUE, 0);

    g_signal_connect (spin, "value-changed", G_CALLBACK (dataWidgetChanged), (gpointer)spin);
    return spin;
}
GtkWidget* createTextBox(const char *name, GtkWidget *boxMenu, const char *str, const char *oName){
    GtkWidget* text = gtk_entry_new();
    gtk_widget_set_name(text, oName);
    gtk_entry_set_max_length(GTK_ENTRY(text), 50);

    GtkWidget *frameText = gtk_frame_new (name);
    //gtk_box_pack_start(GTK_BOX(boxMenu), frameText, TRUE, TRUE, 5);
    gtk_container_add (GTK_CONTAINER (boxMenu), frameText);
    gtk_container_add (GTK_CONTAINER (frameText), text);

    g_signal_connect(G_OBJECT(text), "changed", G_CALLBACK(dataWidgetChanged), (gpointer)text);
    gtk_entry_set_text(GTK_ENTRY(text), str);
    return text;
}

static gboolean configureLayers (GtkWidget *widget, GdkEventConfigure *event, gpointer data)
{
    if (layerBackground)
        cairo_surface_destroy (layerBackground);
    if (layerEdges)
        cairo_surface_destroy (layerEdges);
    if (layerVertices)
        cairo_surface_destroy (layerVertices);
    if (layerMessage)
        cairo_surface_destroy (layerMessage);

    layerBackground = gdk_window_create_similar_surface (gtk_widget_get_window (widget), CAIRO_CONTENT_COLOR_ALPHA,
                                                 gtk_widget_get_allocated_width (widget),
                                                 gtk_widget_get_allocated_height (widget));

    layerEdges = gdk_window_create_similar_surface (gtk_widget_get_window (widget), CAIRO_CONTENT_COLOR_ALPHA,
                                                 gtk_widget_get_allocated_width (widget),
                                                 gtk_widget_get_allocated_height (widget));

    layerVertices = gdk_window_create_similar_surface (gtk_widget_get_window (widget), CAIRO_CONTENT_COLOR_ALPHA,
                                                 gtk_widget_get_allocated_width (widget),
                                                 gtk_widget_get_allocated_height (widget));

    layerMessage = gdk_window_create_similar_surface (gtk_widget_get_window (widget), CAIRO_CONTENT_COLOR_ALPHA,
                                                 gtk_widget_get_allocated_width (widget),
                                                 gtk_widget_get_allocated_height (widget));

    clearSurface ();

    return TRUE;
}
static gboolean drawLayers (GtkWidget *widget, cairo_t *cr, gpointer data)
{
    cairo_set_source_surface (cr, layerBackground, 0, 0);
    cairo_paint (cr);

    cairo_set_source_surface (cr, layerEdges, 0, 0);
    cairo_paint (cr);

    cairo_set_source_surface (cr, layerVertices, 0, 0);
    cairo_paint (cr);

    cairo_set_source_surface (cr, layerMessage, 0, 0);
    cairo_paint (cr);


    return FALSE;
}

static gboolean buttonPressEvent (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    if (layerVertices == nullptr)
        return FALSE;

    //onOffTimer(false);
    myGraph->thisVertex = myGraph->findNearestVertex(event->x, event->y);
    if (event->button == GDK_BUTTON_PRIMARY)
    {
        if(myGraph->thisVertex == nullptr){
            //preDrawGraph(nullptr, drawing_area);
            return FALSE;
        }
        MOVE_THIS_VERTEX = FALSE;

        myGraph->thisVertex->x = event->x;
        myGraph->thisVertex->y = event->y;
        //preDrawGraph((GtkButton*)widget, widget);
    }
    else if (event->button == GDK_BUTTON_MIDDLE)
    {

        if(myGraph->thisVertex == nullptr){
            myGraph->thisEdge = myGraph->findNearestEdge(event->x, event->y);
            if(myGraph->thisEdge == nullptr)
                return FALSE;
            myGraph->deleteEdge(myGraph->thisEdge);
            preDrawGraph(nullptr, drawing_area);
            return FALSE;
        }
        else{
            myGraph->thisEdge = myGraph->addNewEdge(myGraph->thisVertex,
                                                    std::make_shared<VertexClass>(event->x, event->y));

            myGraph->thisVertex = myGraph->thisEdge->B;
        }
    }
    else if (event->button == GDK_BUTTON_SECONDARY)
    {
        if(myGraph->thisVertex == nullptr){
            myGraph->addNewVertex(event->x, event->y);
        }
        else{
            myGraph->deleteVertex(myGraph->thisVertex);
        }
        preDrawGraph(nullptr, drawing_area);
    }
    return TRUE;
}
static gboolean buttonReleaseEvent (GtkWidget *widget, GdkEventButton *event, gpointer user_data)
{
    if (layerVertices == nullptr || myGraph->thisVertex == nullptr)
        return FALSE;

    //onOffTimer(TRUE);
    MOVE_THIS_VERTEX = TRUE;

    if (event->button == GDK_BUTTON_MIDDLE && myGraph->thisEdge != nullptr)
    {
        myGraph->thisEdge->B = myGraph->findNearestVertex(event->x, event->y);
        if(myGraph->thisEdge->A == nullptr || myGraph->thisEdge->B == nullptr)
            myGraph->deleteEdge(myGraph->edges->at(myGraph->edges->size()-1));
    }
    return TRUE;
}
static gboolean motionNotifyEvent (GtkWidget *widget, GdkEventMotion *event, gpointer user_data)
{
    if (layerVertices == nullptr || myGraph->thisVertex == nullptr)
        return FALSE;

    if (event->state & GDK_BUTTON1_MASK){
        myGraph->thisVertex->x = event->x;
        myGraph->thisVertex->y = event->y;
    }
    else if (event->state & GDK_BUTTON2_MASK && myGraph->thisEdge != nullptr){
        myGraph->thisVertex->x = event->x;
        myGraph->thisVertex->y = event->y;
    }

    if(!continue_timer)
        preDrawGraph(nullptr, widget);

    return TRUE;
}



static void close_window (void)
{
    if (layerBackground)
        cairo_surface_destroy (layerBackground);
    if (layerEdges)
        cairo_surface_destroy (layerEdges);
    if (layerVertices)
        cairo_surface_destroy (layerVertices);

    gtk_main_quit();
}


static void activate ()//GtkApplication *app, gpointer user_data
{
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
    gtk_window_maximize(GTK_WINDOW(window));
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_window_set_title (GTK_WINDOW (window), "Graph");
    gtk_container_set_border_width (GTK_CONTAINER (window), 8);

    g_signal_connect (G_OBJECT(window), "destroy", G_CALLBACK (close_window), nullptr);

    GtkWidget *boxMain = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_container_add(GTK_CONTAINER(window), boxMain);

    //left menu
    GtkWidget *boxMenuL = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(boxMain), boxMenuL, TRUE, TRUE, 0);

    //center board
    drawing_area = gtk_drawing_area_new ();
    gtk_widget_set_size_request (drawing_area, WIDTH, HEIGHT);

    GtkWidget *frame = gtk_frame_new (nullptr);
    gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
    gtk_container_add (GTK_CONTAINER (boxMain), frame);
    gtk_container_add (GTK_CONTAINER (frame), drawing_area);
    //gtk_box_pack_start(GTK_BOX(boxMain), drawing_area, TRUE, TRUE, 0);

    //right menu
    char str[20];
    GtkWidget *boxMenuR = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(boxMain), boxMenuR, TRUE, TRUE, 0);
//left

    //GtkWidget *spinVertexRadius;
    createSpinButton("Vertex radius:", 0, MAX_RADIUS, boxMenuL, RADIUS, NameSpinVertexRadius);

    //GtkWidget *spinEdgeWidth;
    createSpinButton("Edge width:", 0, MAX_EDGE_WIDTH, boxMenuL, EDGE_WIDTH, NameSpinEdgeWidth);

    //GtkWidget *spinJump;
    createSpinButton<int>("Iteration jump:", 1, 1000, boxMenuL, JUMP, NameSpinJump);

    createSpinButton("Timer sleep:", 5, MAX_VERTICES, boxMenuL, TIMER_SLEEP, NameSpinTimerSleep);

    GtkWidget *buttonDrawGraph = gtk_button_new_with_label("Draw the next iteration");
    gtk_box_pack_start(GTK_BOX(boxMenuL), buttonDrawGraph,    TRUE, TRUE, 0);
    g_signal_connect(G_OBJECT(buttonDrawGraph), "clicked",  G_CALLBACK(preDrawGraph), drawing_area);
    g_signal_connect(G_OBJECT(buttonDrawGraph), "activate", G_CALLBACK(preDrawGraph), drawing_area);

    GtkWidget *buttonStopAnimation = gtk_button_new_with_label("Stop");
    gtk_widget_set_name(buttonStopAnimation, NameButtonStopAnimation);
    g_signal_connect(G_OBJECT(buttonStopAnimation), "clicked", G_CALLBACK(buttonClicked), drawing_area);
    gtk_box_pack_start(GTK_BOX(boxMenuL), buttonStopAnimation,    TRUE, TRUE, 0);

    GtkWidget *buttonHowToUse = gtk_button_new_with_label("How to use");
    gtk_widget_set_name(buttonHowToUse, NameButtonHowToUse);
    g_signal_connect(G_OBJECT(buttonHowToUse), "clicked", G_CALLBACK(buttonClicked), drawing_area);
    gtk_box_pack_start(GTK_BOX(boxMenuL), buttonHowToUse,    TRUE, TRUE, 0);

//center
    g_signal_connect (drawing_area, "draw", G_CALLBACK (drawLayers), drawing_area);
    g_signal_connect (drawing_area,"configure-event", G_CALLBACK (configureLayers), nullptr);


    g_signal_connect (drawing_area, "motion-notify-event", G_CALLBACK (motionNotifyEvent), nullptr);
    g_signal_connect (drawing_area, "button-press-event",  G_CALLBACK (buttonPressEvent),  nullptr);
    g_signal_connect (drawing_area, "button-release-event",G_CALLBACK (buttonReleaseEvent),nullptr);

    gtk_widget_set_events (drawing_area, gtk_widget_get_events (drawing_area) | GDK_BUTTON_PRESS_MASK
                                                                              | GDK_BUTTON_RELEASE_MASK
                                                                              | GDK_POINTER_MOTION_MASK);


//right
    //GtkWidget *spinNumOfVertices;
    createSpinButton<int>("Vertices:", 2, MAX_VERTICES, boxMenuR, VERTEX_GEN, NameSpinNumOfVertices);

    //GtkWidget *spinNumOfEdge;
    createSpinButton<int>("Edges:", 1, MAX_EDGES, boxMenuR, EDGE_GEN, NameSpinNumOfEdge);

    GtkWidget *buttonGenNewGraph = gtk_button_new_with_label("Generate a graph");
    gtk_widget_set_name(buttonGenNewGraph, NameButtonGenNewGraph);
    g_signal_connect(G_OBJECT(buttonGenNewGraph), "clicked", G_CALLBACK(buttonClicked), drawing_area);
    gtk_box_pack_start(GTK_BOX(boxMenuR), buttonGenNewGraph,    TRUE, TRUE, 0);

    sprintf(str, "%lf", GRAVITY);
    //GtkWidget *textBoxGravity    =
    createTextBox("Gravity force:", boxMenuR, str, NameTextBoxGravity);

    sprintf(str, "%lf", ATTRACTION);
    //GtkWidget *textBoxAttraction =
    createTextBox("Attraction force:", boxMenuR, str, NameTextBoxAttraction);

    sprintf(str, "%lf", REPULSION);
    //GtkWidget *textBoxRepulsion  =
    createTextBox("Repulsion force:", boxMenuR, str, NameTextBoxRepulsion);

    sprintf(str, "%lf", SPEED);
    //GtkWidget *textBoxSpeed      =
    createTextBox("Speed:", boxMenuR, str, NameTextBoxSpeed);


    gtk_widget_show_all(window);
    //timerId = g_timeout_add(TIMER_SLEEP, timeOut, nullptr);
    //continue_timer = TRUE;
    //start_timer = TRUE;
    startTimer();
}
