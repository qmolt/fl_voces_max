#include "ext.h"
#include "ext_obex.h"
#include "z_dsp.h"  

#define MAX_ARRAY 128

enum ARGUMENTS { NONE };
enum INLETS { I_SCALE, I_COLORS, I_MULTIP, NUM_INLETS };
enum OUTLETS { O_OUTPUT, NUM_OUTLETS };

static t_class* fl_voces_class;

typedef struct {

	t_object obj;

	long m_in;

	void* m_proxy1;
	void* m_proxy2;

	long estrucin[MAX_ARRAY];
	long len_est;
	long colors[MAX_ARRAY];
	long len_col;
	long multipliers[MAX_ARRAY];
	long len_mult;

	long mod_base;

	char is_scale_init;
	char is_colors_init;
	char is_multips_init;

	void* m_outlet;

} t_fl_voces;

void* fl_voces_new(t_symbol* s, short argc, t_atom* argv);
void fl_voces_assist(t_fl_voces* x, void* b, long msg, long arg, char* dst);

void fl_voces_out(t_fl_voces* x);
void fl_voces_lista(t_fl_voces* x, t_symbol* s, long argc, t_atom* argv);

void fl_voces_mod_base(t_fl_voces* x, t_symbol* s, long argc, t_atom* argv);