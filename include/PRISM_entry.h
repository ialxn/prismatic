//
// Created by AJ Pryor on 3/2/17.
//

#ifndef PRISM_PRISM_ENTRY_H
#define PRISM_PRISM_ENTRY_H
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include "configure.h"
#include "ArrayND.h"
#include "PRISM01.h"
#include "PRISM02.h"
#include "PRISM03.h"
#include "params.h"
#include <vector>

namespace PRISM{

	template <class T>
	using Array3D = ArrayND<3, std::vector<T> >;
	template <class T>
	using Array2D = ArrayND<2, std::vector<T> >;
	template <class T>
	using Array1D = ArrayND<1, std::vector<T> >;

	inline int PRISM_entry(Metadata<PRISM_FLOAT_PRECISION>& meta){
		Parameters<PRISM_FLOAT_PRECISION> prism_pars(meta);


		PRISM01(prism_pars);
//		cout << prism_pars.pot.at(0,0,0);
		prism_pars.pot.toMRC_f("test.mrc");
		PRISM02(prism_pars);
//		PRISM03(prism_pars);
//
//		size_t lower = 13;
//		size_t upper = 18;
//		Array2D<PRISM_FLOAT_PRECISION> prism_image;
//		prism_image = zeros_ND<2, PRISM_FLOAT_PRECISION>({{prism_pars.stack.get_diml(), prism_pars.stack.get_dimk()}});
//		for (auto y = 0; y < prism_pars.stack.get_diml(); ++y){
//			for (auto x = 0; x < prism_pars.stack.get_dimk(); ++x){
//				for (auto b = lower; b < upper; ++b){
//					prism_image.at(y,x) += prism_pars.stack.at(y,x,b,1);
//				}
//			}
//		}
//
//		prism_image.toMRC_f(prism_pars.meta.filename_output.c_str());
//		std::cout << "Calculation complete.\n" << std::endl;
		return 0;
	}

}
#endif //PRISM_PRISM_ENTRY_H
