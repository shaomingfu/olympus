#include "group.h"

int main(int argc, const char ** argv)
{
	if(argc != 4) return 0;

	group gr;
	gr.load(argv[1]);
	gr.forbid(argv[2]);
	gr.replace(argv[3]);
	gr.parse();
	//gr.build();
	gr.clust();
	return 0;
}
