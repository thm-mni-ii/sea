#ifndef SEALIB_INPLACEBFS_H_
#define SEALIB_INPLACEBFS_H_

#include <memory>

#include "sealib/_types.h"
#include "sealib/compactarray.h"
#include "sealib/compactgraph.h"

#define BFS_WHITE 0
#define BFS_LIGHTGRAY 1
#define BFS_DARKGRAY 2
#define BFS_BLACK 3

typedef void (*UserFunc1)(uint);
typedef void (*UserFunc2)(uint, uint);

#define BFS_NOP_PROCESS (UserFunc1)0
#define BFS_NOP_EXPLORE (UserFunc2)0

namespace Sealib{
	class ChoiceDictionaryDummy: public CompactArray{
		public:
			ChoiceDictionaryDummy(uint n);
			void init(uint color);
			bool more();
			uint next();
			void setColor(uint color,uint v);
			uint color(uint v);
		private:
			uint current_position;
			uint current_color;
			uint size;
	};

	class InplaceBFS {
		private:
			Compactgraph *g;
			ChoiceDictionaryDummy *color;
			uint round_number;
			UserFunc1 preprocess;
			UserFunc2 preexplore;
			/* Returns the pointer to the adjacency array of i 
			 * @param i = index of Node 
			 */
		public:
			uint read(uint i);
			InplaceBFS(Compactgraph *g, ChoiceDictionaryDummy *color, UserFunc1 pp, UserFunc2 pe);
			void runInplaceBFS();

	};
} //namespace Sealib

#endif
