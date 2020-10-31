#include "flvoces.h"

void C74_EXPORT main() {

	t_class *c;

	c = class_new("flvoces", (method)fl_voces_new, (method)NULL, sizeof(t_fl_voces), 0, 0);
	class_addmethod(c, (method)fl_voces_assist,"assist", A_CANT, 0);

	class_addmethod(c, (method)fl_voces_out, "bang", 0);
	class_addmethod(c, (method)fl_voces_lista, "list", A_GIMME, 0);
	class_addmethod(c, (method)fl_voces_mod_base, "mod_base", A_GIMME, 0);

	class_register(CLASS_BOX, c);
	fl_voces_class = c; 	
}

void *fl_voces_new(t_symbol *s, short argc, t_atom *argv) 
{
	t_fl_voces *x = (t_fl_voces *)object_alloc(fl_voces_class);

	x->m_proxy2 = proxy_new((t_object *)x, 2, &x->m_in);
	x->m_proxy1 = proxy_new((t_object *)x, 1, &x->m_in);

	x->m_outlet = outlet_new((t_object *)x,NULL);				

	x->len_col = MAX_ARRAY;
	x->len_mult = MAX_ARRAY;
	x->len_est = MAX_ARRAY;
	for (long i = 0; i < MAX_ARRAY; i++) {
		x->estrucin[i] = 0;
		x->colors[i] = 0;
		x->multipliers[i] = 0;
	}

	x->mod_base = 12;

	x->is_scale_init = 0;
	x->is_colors_init = 0;
	x->is_multips_init = 0;

	return x;
}

void fl_voces_assist(t_fl_voces *x, void *b, long msg, long arg, char *dst)
{
	if (msg == ASSIST_INLET) {										
		switch (arg) {
		case I_SCALE: sprintf(dst, "(list) scale estructure"); break;
		case I_COLORS:	sprintf(dst, "(list) colors"); break;
		case I_MULTIP:	sprintf(dst, "(list) multipliers"); break;
		}
	}
	else if (msg == ASSIST_OUTLET) {    
		switch (arg) {
		case O_OUTPUT: sprintf(dst, "(list) scale"); break;
		}
	}
}

void fl_voces_lista(t_fl_voces* x, t_symbol* s, long argc, t_atom* argv)
{
	long ac = argc;
	t_atom* ap = argv;
	long mod_base = x->mod_base;
	long scale_size = x->len_est;

	switch (proxy_getinlet((t_object*)x)) {
	case I_SCALE:
		for (long i = 0; i < ac; i++) {
			x->estrucin[i] = (long)atom_getlong(ap + i);
			if (i > 0) {
				if (x->estrucin[i - 1] > x->estrucin[i]) {
					x->estrucin[i] += mod_base;
				} 
			}
		}
		x->len_est = ac;
		x->is_scale_init = 1;
		fl_voces_out(x);
		break;
	case I_COLORS:
		for (long i = 0; i < ac; i++) {
			x->colors[i] = (long)atom_getlong(ap + i);
			x->colors[i] = x->colors[i] % scale_size;
		}
		x->len_col = ac;
		x->is_colors_init = 1;
		break;
	case I_MULTIP:
		for (long i = 0; i < ac; i++) {
			x->multipliers[i] = (long)atom_getlong(ap + i);
		}
		x->len_mult = ac;
		x->is_multips_init = 1;
		break;
	}
}

void fl_voces_out(t_fl_voces* x)
{
	long* pscale;
	long* pcolors;
	long* pmultip;
	t_atom estrucout[MAX_ARRAY];
	t_atom* atomout;
	long scale_size = x->len_est;
	long total_colors = x->len_col;
	long total_multip = x->len_mult;
	long mod_base = x->mod_base;
	long val;
	char is_scale_init = x->is_colors_init;
	char is_colors_init = x->is_colors_init;
	char is_multips_init = x->is_multips_init;

	pscale = x->estrucin;
	pcolors = x->colors;
	pmultip = x->multipliers;
	atomout = estrucout;

	if (total_colors != total_multip) { object_error((t_object*)x, "colors list and multipliers list must be the same size"); return; }

	if (is_scale_init && is_colors_init && is_multips_init) {
		for (long i = 0; i < total_colors; i++) {
			val = pmultip[i] * mod_base + pscale[pcolors[i]];
			atom_setlong(estrucout + i, val);
		}
		outlet_list(x->m_outlet, NULL, (short)total_colors, estrucout);
	}
}

void fl_voces_mod_base(t_fl_voces* x, t_symbol* s, long argc, t_atom* argv)
{
	long ac = argc;
	t_atom* ap = argv;
	long mod_base;

	if (ac != 1) { object_error((t_object*)x, "[1 arg] modulo base"); return; }

	if (atom_gettype(ap) != A_LONG && atom_gettype(ap) != A_FLOAT) { object_error((t_object*)x, "modulo base must be a number"); return; }

	mod_base = (long)atom_getlong(ap);

	if (mod_base <= 0) { object_error((t_object*)x, "modulo base must be a positive number"); return; }

	x->mod_base = mod_base;
}