//
//  ILPSolver.cpp
//  yoshiko
//
//  Created by Gunnar Klau on 15-06-12.
//  Copyright (c) 2012 Centrum Wiskunde & Informatica (CWI). All rights reserved.
//

#include "ILPSolver.h"

using namespace std;
using namespace yskLib;
using namespace lemon;

namespace ysk {

/*************************************************************************
 * This callback procedure is called by Cplex during the computation
 * of the ILP.
 *************************************************************************/
ILOMIPINFOCALLBACK1(gap_callback,yskLib::CplexInformer*, informer) {
	double gap = getMIPRelativeGap();
	if (verbosity >= 3) cout << "Informing listener about new gap value: " << gap << endl;
    informer->updateStatus(SOLVING_ILP,gap);
    return;
}

/*************************************************************************
 * This callback procedure is called by Cplex during the computation
 * of the ILP.
 *************************************************************************/
ILOLAZYCONSTRAINTCALLBACK2(triangle_callback, const ClusterEditingInstance&, inst,  const IloBoolVarArray&, x) {

    IloEnv env = getEnv();
    //const double epsilon = IloCplex::EpInt;  gunnar: EpInt ist falsch, aber vielleicht was anderes
    //cout << "in callback "  << endl;
    try {

        int no_added = 0, no_added_triplet_cuts = 0; // no cuts added yet in this iteration
        //bool fractional = false;

        IloNumArray x_vals(env);
        getValues(x_vals, x);

        const FullGraph g = inst.getOrig();
        const int n = g.nodeNum();

        // round LP solution a bit:

        for (FullGraph::EdgeIt e(g); e != INVALID; ++e) {
            if (x_vals[g.id(e)] < eps) x_vals[g.id(e)] = 0.0;
            if (x_vals[g.id(e)] > 1.0 - eps) x_vals[g.id(e)] = 1.0;
            //if (x_vals[g.id(e)] > eps && x_vals[g.id(e)] < 1.0 - eps) fractional = true;
        }

        unsigned long long no_checks = n * (n-1) * (n-2) / 6, no_checked = 0;
        //	   cout << " versus " << no_checks << endl;

        for (FullGraph::NodeIt i(g); i != INVALID; ++i) {
            if (time_limit != -1) {
                if (clk.systemTime() > time_limit) //TOOD: Coherent use of time_limit in ALL of yoshiko
                    throw("time limit exceeded in yoshiko triangle callback");

                cout << clk << endl;
            }

            FullGraph::NodeIt j(g); j = i;
            for (++j; j != INVALID; ++j) {
                if (verbosity > 1) // && ((no_checked++ % 10000) == 0))
                    cout << "\rchecking triangle inequalities... (" << (no_checked)/double(no_checks) * 100
                    << " %) added " << no_added << "              " << flush;

                FullGraph::NodeIt k(g); k = j;
                //      for (++k; k != INVALID && no_added < 100000; ++k) { // if (k != i && k != j) {
                for (++k; k != INVALID; ++k) { // if (k != i && k != j) {
                    ++no_checked;
                    if (x_vals[g.id(g.edge(i, j))] + x_vals[g.id(g.edge(j, k))] - x_vals[g.id(g.edge(i, k))] > 1 + 3 * eps) {
                        add(x[g.id(g.edge(i, j))] + x[g.id(g.edge(j, k))] - x[g.id(g.edge(i, k))] <= 1);
                        ++no_added; ++no_added_triplet_cuts;
                    }

                    if (x_vals[g.id(g.edge(i, j))] - x_vals[g.id(g.edge(j, k))] + x_vals[g.id(g.edge(i, k))] > 1 + 3 * eps) {
                        add(x[g.id(g.edge(i, j))] - x[g.id(g.edge(j, k))] + x[g.id(g.edge(i, k))] <= 1);
                        ++no_added; ++no_added_triplet_cuts;
                    }

                    if (-x_vals[g.id(g.edge(i, j))] + x_vals[g.id(g.edge(j, k))] + x_vals[g.id(g.edge(i, k))] > 1 + 3 * eps) {
                        add(-x[g.id(g.edge(i, j))] + x[g.id(g.edge(j, k))] + x[g.id(g.edge(i, k))] <= 1);
                        ++no_added; ++no_added_triplet_cuts;
                    }
                }
            }
        }

        if (verbosity > 1)
            cout << "done." << endl;

        return;
    }
    catch (IloException& e) {
        env.out() << e << endl;
        throw -1;
    }
    catch (Exception& e) {
        env.out() << e.what() << endl;
        throw -1;
    }
    catch (...) {
        env.out() << "something strange in separation callback" << endl;
        throw -1;
    }
}

/*************************************************************************
 * This callback procedure is called by Cplex during the computation
 * of the ILP.
 *************************************************************************/
ILOUSERCUTCALLBACK2(partition_cut_callback, const ClusterEditingInstance&, inst,  const IloBoolVarArray&, x) {

    IloEnv env = getEnv();
    //const double epsilon = IloCplex::EpInt;  gunnar: EpInt ist falsch, aber vielleicht was anderes
    cout << "in callback "  << endl;
    try {

        int no_added = 0; // no cuts added yet in this iteration
        //bool fractional = false;

        IloNumArray x_vals(env);
        getValues(x_vals, x);

        const FullGraph g = inst.getOrig();

        // round LP solution a bit:

        for (FullGraph::EdgeIt e(g); e != INVALID; ++e) {
            if (x_vals[g.id(e)] < eps) x_vals[g.id(e)] = 0.0;
            if (x_vals[g.id(e)] > 1.0 - eps) x_vals[g.id(e)] = 1.0;
            //if (x_vals[g.id(e)] > eps && x_vals[g.id(e)] < 1.0 - eps) fractional = true;
        }

        // we do have fractional values:
        //cout << "fractional solution in callback..." << endl;

        // heuristic 2
        int no_added_partition_cuts = 0; int cnt_i = 0;
        for (FullGraph::NodeIt i(g); i != INVALID; ++i) {
            list<FullGraph::Node> W;
            for (FullGraph::NodeIt j(g); j != INVALID; ++j) if (i < j)
                if (x_vals[g.id(g.edge(i, j))] > 0.0)
                    W.push_back(j);

            // permute??? warum???

            for (list<FullGraph::Node>::const_iterator cit_j = W.begin(); cit_j != W.end(); ++cit_j) {
                FullGraph::Node j = *cit_j;
                IloExpr cut(env);
                double cut_value = 0.0;

                list<FullGraph::Node> T;
                T.push_back(j);
                int idx_ij = g.id(g.edge(i, j));
                cut += x[idx_ij];
                cut_value += x_vals[idx_ij];

                for (list<FullGraph::Node>::const_iterator cit_k = W.begin(); cit_k != W.end(); ++cit_k) if (*cit_k != j) {
                    FullGraph::Node k = *cit_k;
                    double take_k = 0.0;

                    for (list<FullGraph::Node>::const_iterator cit_l = T.begin(); cit_l != T.end(); ++cit_l)
                        take_k += x_vals[g.id(g.edge(k, *cit_l))];

                    //obacht: check .05
                    if (x_vals[g.id(g.edge(i, k))] - take_k > .05) {
                        T.push_back(k);
                        cut += x[g.id(g.edge(i, k))];
                        cut_value += x_vals[g.id(g.edge(i, k))];
                    }
                }

                for (list<FullGraph::Node>::const_iterator cit_t1 = T.begin(); cit_t1 != T.end(); ++cit_t1)
                    for (list<FullGraph::Node>::const_iterator cit_t2 = T.begin(); cit_t2 != T.end(); ++cit_t2) if (*cit_t1 < *cit_t2) {
                        cut -= x[g.id(g.edge(*cit_t1, *cit_t2))];
                        cut_value -= x_vals[g.id(g.edge(*cit_t1, *cit_t2))];
                    }

                // check whether ineq is violated:
                if (cut_value > 1.0 + eps && cut_value/((double) T.size()) > .3) {
                    //cout <<  cut_value/((double) T.size()) << endl;
                    //cout << cut << " = " << cut_value << endl;
                    //cout << "+";
                    no_added++; ++no_added_partition_cuts;
                    add(cut <= 1);
                    cut.end();
                }
            }
            if (verbosity > 1) {
                cout << "\rchecked partition inequalities... (" << ++cnt_i << "/" << g.nodeNum() << ")   cuts added: " << no_added_partition_cuts << flush;
            }
        }
        if (verbosity > 1)
            cout << endl;

        return;
    }
    catch (IloException& e) {
        env.out() << e << endl;
        throw -1;
    }
    catch (Exception& e) {
        env.out() << e.what() << endl;
        throw -1;
    }
    catch (...) {
        env.out() << "something strange in separation callback" << endl;
        throw -1;
    }
}

void ILPSolver::registerInformer(yskLib::CplexInformer* informer){
	if (verbosity > 2) cout << "Registered CplexInformer @ ILPSolver" << endl;
	_informer = informer;
}


void ILPSolver::terminate(){
	if (verbosity > 2){
		cout << "Terminating CPLEX Environment" << endl;
	}
	if (_cplexInitialized){
		_aborter.abort();
	}
}

long ILPSolver::solve(const ClusterEditingInstance& inst, ClusterEditingSolutions& s, SolutionFlags& flags) {
	if(inst.isDirty()) {
        cerr << "Fatal error: ClusterEditingInstance is dirty."<<endl;
        exit(-1);
    }

    // get graph, I need it so often
    const FullGraph g = inst.getOrig();

    //skip if size is 1 because cplex throws an error in this case.
    //@gunnar, vielleicht kann man das auch schöner umgehen...
    if(g.nodeNum() <= 1) {
        if (verbosity > 1) {
            cout << "size of instance is 1. No further computation is necessary."<<endl;
        }
        s.resize(1);
        s.setSolution(0, inst);
        return 1;
    }

    // first build ILOG model...
    IloEnv cplexEnv; // get ILOG environment
    IloModel M(cplexEnv); // get model
    IloCplex cplex(cplexEnv); // get cplex

    _aborter = IloCplex::Aborter(cplexEnv); //generate Aborter object and link to cplex
    cplex.use(_aborter);
    _cplexInitialized = true; //flag for external objects to check if a cplex instance is initialized (and can thus be stopped)

    // shut up cplex
    if (verbosity < 3) {
    	cplex.setOut(cplexEnv.getNullStream());
    	cplex.setWarning(cplexEnv.getNullStream());
    	cplex.setError(cplexEnv.getNullStream());
    }
    else{
    	cplex.setParam(IloCplex::MIPDisplay,5);
    	cplex.setOut(cout);
    }

    if (no_threads != -1)
    	cplex.setParam(IloCplex::Threads, no_threads);

    // set CPU time limit
    cplex.setParam(IloCplex::ClockType, 1);
    if (time_limit != -1)
    	cplex.setParam(IloCplex::TiLim, time_limit);


    //Limit Memory usage as this runs in a GUI and we should still be able to use the GUI
    cplex.setParam(IloCplex::NodeFileInd,2);


    // set all generic cuts off
    cplex.setParam(IloCplex::Cliques, -1);
    cplex.setParam(IloCplex::Covers, -1);
    cplex.setParam(IloCplex::DisjCuts, -1);
    cplex.setParam(IloCplex::FlowCovers, -1);
    cplex.setParam(IloCplex::FracCuts, -1);
    cplex.setParam(IloCplex::GUBCovers, -1);
    cplex.setParam(IloCplex::Cliques, -1);
    cplex.setParam(IloCplex::ImplBd, -1);
    cplex.setParam(IloCplex::MIRCuts, -1);
    cplex.setParam(IloCplex::FlowPaths, -1);

    int n = g.nodeNum();
    if (verbosity > 1) {
        cout << n << " nodes" << endl;
    }

    // generate variables:

    IloBoolVarArray x(cplexEnv, g.edgeNum());
    for (FullGraph::EdgeIt e(g); e != INVALID; ++e) {
        std::stringstream var_name;
        var_name << "x_" << g.id(g.u(e)) << "_" << g.id(g.v(e));
        x[g.id(e)] = IloBoolVar(cplexEnv, var_name.str().c_str());
        M.add(x[g.id(e)]);
        //cout << g.id(g.source(a)) << " --> " << g.id(g.target(a)) << "\t" << g.id(a) << " " << g.edge_no(g.id(g.source(a)), g.id(g.target(a))) << endl;
    }


    // build objective function
    IloExpr obj_expr(cplexEnv);
    for (FullGraph::EdgeIt e(g); e != INVALID; ++e)
        if (!inst.isForbidden(e) && !inst.isPermanent(e)) {
            obj_expr -= inst.getWeight(e) * x[g.id(e)];
            if (inst.getWeight(e) > 0)
                obj_expr += inst.getWeight(e);
        }

    M.add(IloObjective(cplexEnv, obj_expr, IloObjective::Minimize));

    // add inequalities for forbidden/permanent edges:
    for (FullGraph::EdgeIt e(g); e != INVALID; ++e) {
        if (inst.isForbidden(e))
            M.add(x[g.id(e)] == 0);

        if (inst.isPermanent(e))
            M.add(x[g.id(e)] == 1);
    }


    //CALLBACKS

    if (_sep_triangles) cplex.use(triangle_callback(cplexEnv, inst, x)); // use triangle callback --> lazy constraints!
    if (_sep_partition_cuts) cplex.use(partition_cut_callback(cplexEnv, inst, x)); // use partition callback --> user cuts!
    //if we're using an informer to report to any outside source we will register it here
    if (_informer != nullptr){
    	cplex.use(gap_callback(cplexEnv,_informer));
    	//TODO: Maybe use even more callbacks to provide more info
    }

    if (!_sep_triangles) {
        if (verbosity > 1) {
            cout << "adding triangle inequalities... " << flush;
        }

        for (FullGraph::NodeIt i(g); i != INVALID; ++i) {
            FullGraph::NodeIt j(g); j = i;
            for (++j; j != INVALID; ++j) {
                FullGraph::NodeIt k(g); k = j;
                for (++k; k != INVALID; ++k) {
                    M.add( x[g.id(g.edge(i, j))] + x[g.id(g.edge(j, k))] - x[g.id(g.edge(i, k))] <= 1);
                    M.add( x[g.id(g.edge(i, j))] - x[g.id(g.edge(j, k))] + x[g.id(g.edge(i, k))] <= 1);
                    M.add(-x[g.id(g.edge(i, j))] + x[g.id(g.edge(j, k))] + x[g.id(g.edge(i, k))] <= 1);
                }
            }
        }
    }

    if (verbosity > 1)
        cout << "done." << endl;


    //cout << "extracting (and exporting) ILP... " << flush;
    if (verbosity > 1) {
        cout << "extracting ILP... " << flush;
    }

    cplex.extract(M);
    //if (verbose) cplex.exportModel("yoshiko.lp");
    if (verbosity > 1) {
        cout << "done." << endl;
    }

    if (verbosity > 1) {
        cout << "solving ILP (computing one optimal solution)... " << flush;
    }

    //Start Solver, returns true if the solution is feasible (not optimal!)
	flags.ilpGenerated = true; //Mark solution as being generated by ILP
    bool feasible = cplex.solve();

    //Time-out handling
    if (cplex.getCplexStatus() == IloCplex::AbortTimeLim){
    	bool resume = false;
    	//If we use the informer we have the possibility of asking the listener to resume the solver
    	if (_informer != nullptr){
    		if (verbosity > 1) cout << "Requesting further instructions ..." << endl;
    		if (_informer->continueOnTimeout()){
    			if (verbosity > 2) cout << "Continuing at users request!" << endl;
    			resume = true;
    			cplex.setParam(IloCplex::TiLim, IloCplex::IntParam_MAX);
    			feasible = cplex.solve();
    		}
    	}
    	if (!resume){
        	cout << "[CPLEX TIME OUT]" << endl;
        	flags.timedOut =  true;
    	}

    }

    //For some reason the solver terminated without providing an optimal solution
    if (!feasible) {
        cout << endl << endl << endl;
        cout << cplex.getStatus() << endl;
        cout << endl << endl << endl;
        cerr << "yoshiko: Optimization problems. CPLEX status code " << cplex.getStatus() << endl;
    }
    else if (cplex.getCplexStatus() == IloCplex::Optimal){
    	flags.optimal = true;
    }

    //Assign cost to the solution
    double z = cplex.getObjValue();
    flags.totalCost = z;
    if(verbosity >1){
        cout << "Total Cost: " << z << endl;
    }

    //Add Gap for instance
    flags.instances.push_back(cplex.getMIPRelativeGap());

    //Some output
    if (verbosity > 1) {
        cout << "CPLEX status code " << cplex.getStatus() << endl;
        cout << "upper bound: " << z << endl;
        cout << "lower bound: " << cplex.getBestObjValue() << endl;
        cout << "gap (%):     " << cplex.getMIPRelativeGap()*100 << endl;
    }


    //Fetch additional solution
    long numsol = 1;
    if (_num_opt_sol > 1) {
    	cplex.setParam(IloCplex::SolnPoolGap, 0.0); //
    	cplex.setParam(IloCplex::SolnPoolIntensity, 4); // "all" (sub)optimal solutions
    	cplex.setParam(IloCplex::PopulateLim, _num_opt_sol);


        if (verbosity > 1)
            cout << "solving ILP (finding more optimal solutions)..." << flush;
        cplex.populate();

        if (verbosity > 1)
            cout << "done." << endl;

        numsol = cplex.getSolnPoolNsolns();
        if (verbosity > 1)
            cout  << numsol << " optimal solutions." << endl;
    }

    //Add solutions to return instance
    s.resize(numsol);
    for (int k = 0; k < numsol; ++k) {
        IloNumArray x_vals(cplexEnv);
        cplex.getValues(x_vals, x, k);
        s.setSolution(k, x_vals, inst);
    }

    //CLEANUP!
    cplexEnv.end();

    return numsol;
}

} // namespace ysk
