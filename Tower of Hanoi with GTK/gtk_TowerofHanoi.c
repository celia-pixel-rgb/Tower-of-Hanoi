#include <gtk/gtk.h>   // GTK library (GUI elements)
#include <math.h>      // Math functions
#include <string.h>    // memset
#include <stdio.h>     // snprintf

/* ============================================================
   CONSTANTS (fixed values used throughout the program)
   ============================================================ */

#define MAX_DISKS   7
#define NUM_PEGS    3
#define MAX_MOVES 200

#define CANVAS_W   700
#define CANVAS_H   420

#define PEG_BASE_Y  370
#define PEG_HEIGHT  260
#define DISK_HEIGHT  22
#define PEG_WIDTH     8

/* X positions of the 3 pegs on screen */
static const int PEG_X[NUM_PEGS] = {130, 350, 570};

/* Colors for each disk (RGB format) */
static const double DISK_COLORS[MAX_DISKS][3] = {
    {0.95,0.26,0.21}, {1.00,0.60,0.00}, {1.00,0.85,0.00},
    {0.30,0.69,0.31}, {0.13,0.59,0.95}, {0.40,0.23,0.72},
    {0.91,0.12,0.39}
};

/* A move = take disk from one peg to another */
typedef struct {
    int from;
    int to;
} Move;

/* ============================================================
   GLOBAL VARIABLES (shared across functions)
   ============================================================ */

/* Game state */
static int pegs[NUM_PEGS][MAX_DISKS]; // stores disks
static int height[NUM_PEGS];          // number of disks per peg
static int n_disks;                   // chosen number of disks

/* Move system */
static Move move_list[MAX_MOVES]; // precomputed solution
static int total_moves;
static int current_move;

/* GTK widgets */
static GtkWidget *drawing_area;
static GtkWidget *status_label;
static GtkWidget *next_button;

/* ============================================================
   FUNCTION: generate_moves
   PURPOSE:
   Compute all moves recursively BEFORE the game starts
   ============================================================ */
static void generate_moves(int n, int from, int to, int aux)
{
    /* Base case: only 1 disk ? direct move */
    if (n == 1) {
        move_list[total_moves++] = (Move){from, to};
        return;
    }

    /* Move n-1 disks to auxiliary peg */
    generate_moves(n - 1, from, aux, to);

    /* Move biggest disk */
    move_list[total_moves++] = (Move){from, to};

    /* Move n-1 disks onto it */
    generate_moves(n - 1, aux, to, from);
}

/* ============================================================
   FUNCTION: init_puzzle
   PURPOSE:
   Reset everything to initial state
   ============================================================ */
static void init_puzzle(void)
{
    /* Clear all pegs */
    memset(pegs, 0, sizeof(pegs));
    memset(height, 0, sizeof(height));

    /* Put all disks on peg 0 (left) */
    for (int i = n_disks; i >= 1; i--) {
        pegs[0][height[0]++] = i;
    }

    /* Generate solution */
    total_moves = 0;
    current_move = 0;
    generate_moves(n_disks, 0, 2, 1);
}

/* ============================================================
   FUNCTION: do_next_move
   PURPOSE:
   Execute ONE move from the solution list
   ============================================================ */
static void do_next_move(void)
{
    if (current_move >= total_moves) return;

    int from = move_list[current_move].from;
    int to   = move_list[current_move].to;

    /* Remove disk from source peg */
    int disk = pegs[from][--height[from]];
    pegs[from][height[from]] = 0;

    /* Place disk on destination peg */
    pegs[to][height[to]++] = disk;

    current_move++;
}

/* ============================================================
   FUNCTION: on_draw (GTK4 version)
   PURPOSE:
   Draw everything on screen
   ============================================================ */
static void on_draw(GtkDrawingArea *area,
                    cairo_t *cr,
                    int width,
                    int height_px,
                    gpointer data)
{
    /* Background */
    cairo_set_source_rgb(cr, 0.12, 0.12, 0.16);
    cairo_paint(cr);

    /* Floor */
    cairo_set_source_rgb(cr, 0.55, 0.55, 0.60);
    cairo_set_line_width(cr, 3);
    cairo_move_to(cr, 20, PEG_BASE_Y);
    cairo_line_to(cr, CANVAS_W - 20, PEG_BASE_Y);
    cairo_stroke(cr);

    /* Draw pegs */
    for (int p = 0; p < NUM_PEGS; p++) {

        cairo_set_source_rgb(cr, 0.7, 0.7, 0.75);
        cairo_rectangle(cr,
            PEG_X[p] - PEG_WIDTH / 2,
            PEG_BASE_Y - PEG_HEIGHT,
            PEG_WIDTH,
            PEG_HEIGHT);
        cairo_fill(cr);

        char label[4];
        snprintf(label, sizeof(label), "%d", p + 1);

        cairo_set_source_rgb(cr, 0.8, 0.8, 0.85);
        cairo_set_font_size(cr, 16);
        cairo_move_to(cr, PEG_X[p] - 5, PEG_BASE_Y + 24);
        cairo_show_text(cr, label);
    }

    /* Draw disks */
    for (int p = 0; p < NUM_PEGS; p++) {
        for (int i = 0; i < height[p]; i++) {

            int disk = pegs[p][i];
            if (!disk) continue;

            int disk_w = 30 + disk * ((PEG_X[1] - PEG_X[0] - 60) / MAX_DISKS);
            int disk_y = PEG_BASE_Y - (i + 1) * DISK_HEIGHT - i * 3;
            int disk_x = PEG_X[p] - disk_w / 2;

            int idx = disk - 1;
            if (idx >= MAX_DISKS) idx = MAX_DISKS - 1;

            double r = DISK_COLORS[idx][0];
            double g = DISK_COLORS[idx][1];
            double b = DISK_COLORS[idx][2];

            /* Draw rectangle */
            cairo_set_source_rgb(cr, r, g, b);
            cairo_rectangle(cr, disk_x, disk_y, disk_w, DISK_HEIGHT);
            cairo_fill(cr);
        }
    }
}

/* ============================================================
   BUTTON CLICK HANDLER
   ============================================================ */
static void on_next_clicked(GtkWidget *button, gpointer data)
{
    if (current_move >= total_moves) return;

    do_next_move();

    char msg[100];
    snprintf(msg, sizeof(msg),
             "Move %d / %d",
             current_move, total_moves);

    gtk_label_set_text(GTK_LABEL(status_label), msg);
    gtk_widget_queue_draw(drawing_area);

    if (current_move >= total_moves) {
        gtk_button_set_label(GTK_BUTTON(next_button), "Solved!");
        gtk_widget_set_sensitive(next_button, FALSE);
    }
}

/* ============================================================
   SPIN BUTTON HANDLER
   ============================================================ */
static void on_disk_changed(GtkSpinButton *spin, gpointer data)
{
    n_disks = gtk_spin_button_get_value_as_int(spin);

    init_puzzle();

    gtk_widget_set_sensitive(next_button, TRUE);
    gtk_button_set_label(GTK_BUTTON(next_button), "Next Move");

    gtk_widget_queue_draw(drawing_area);
}

/* ============================================================
   GTK APPLICATION START
   ============================================================ */
static void activate(GtkApplication *app, gpointer data)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Tower of Hanoi");

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 8);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    /* Controls */
    GtkWidget *hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_append(GTK_BOX(vbox), hbox);

    GtkWidget *spin = gtk_spin_button_new_with_range(1, MAX_DISKS, 1);
    gtk_box_append(GTK_BOX(hbox), spin);

    next_button = gtk_button_new_with_label("Next Move");
    gtk_box_append(GTK_BOX(hbox), next_button);

    /* Drawing area */
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_size_request(drawing_area, CANVAS_W, CANVAS_H);
    gtk_box_append(GTK_BOX(vbox), drawing_area);

    /* Status */
    status_label = gtk_label_new("Ready");
    gtk_box_append(GTK_BOX(vbox), status_label);

    /* Connect signals */
    gtk_drawing_area_set_draw_func(GTK_DRAWING_AREA(drawing_area),
                                  on_draw, NULL, NULL);

    g_signal_connect(next_button, "clicked",
                     G_CALLBACK(on_next_clicked), NULL);

    g_signal_connect(spin, "value-changed",
                     G_CALLBACK(on_disk_changed), NULL);

    /* Init */
    n_disks = 4;
    init_puzzle();

    gtk_widget_show(window);
}

/* ============================================================
   MAIN FUNCTION
   ============================================================ */
int main(int argc, char *argv[])
{
    GtkApplication *app =
        gtk_application_new("com.example.hanoi",
                            G_APPLICATION_DEFAULT_FLAGS);

    g_signal_connect(app, "activate",
                     G_CALLBACK(activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);

    g_object_unref(app);
    return status;
}
