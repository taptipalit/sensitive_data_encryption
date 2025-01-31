//===- ConsGEdge.h -- Constraint graph edge-----------------------------------//
//
//                     SVF: Static Value-Flow Analysis
//
// Copyright (C) <2013-2017>  <Yulei Sui>
//

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//===----------------------------------------------------------------------===//

/*
 * ConsGEdge.h
 *
 *  Created on: Mar 19, 2014
 *      Author: Yulei Sui
 */

#ifndef CONSGEDGE_H_
#define CONSGEDGE_H_

#include "llvm/Analysis/SVF/MemoryModel/PAG.h"
#include "llvm/Analysis/SVF/Util/WorkList.h"

#include <llvm/ADT/STLExtras.h>
#include <llvm/ADT/GraphTraits.h>
#include <llvm/IR/CallSite.h>

#include <map>
#include <set>

class ConstraintNode;
/*!
 * Self-defined edge for constraint resolution
 * including add/remove/re-target, but all the operations do not affect original PAG Edges
 */
typedef GenericEdge<ConstraintNode> GenericConsEdgeTy;
class ConstraintEdge : public GenericConsEdgeTy {

public:
    /// five kinds of constraint graph edges
    /// Gep edge is used for field sensitivity
    enum ConstraintEdgeK {
        Addr, Copy, Store, Load, NormalGep, VariantGep, StoreVal, LoadVal, CallVal, RetVal
    };
private:
    EdgeID edgeId;
    bool pruned;
public:
    /// Constructor
    ConstraintEdge(ConstraintNode* s, ConstraintNode* d, ConstraintEdgeK k, EdgeID id = 0) : GenericConsEdgeTy(s,d,k),edgeId(id),pruned(false) {
    }
    /// Destructor
    ~ConstraintEdge() {
    }
    /// Return edge ID
    inline EdgeID getEdgeID() const {
        return edgeId;
    }
    /// ClassOf
    static inline bool classof(const GenericConsEdgeTy *edge) {
        return edge->getEdgeKind() == Addr ||
               edge->getEdgeKind() == Copy ||
               edge->getEdgeKind() == Store ||
               edge->getEdgeKind() == Load ||
               edge->getEdgeKind() == NormalGep ||
               edge->getEdgeKind() == VariantGep ||
               edge->getEdgeKind() == StoreVal ||
               edge->getEdgeKind() == LoadVal;
    }
    /// Constraint edge type
    typedef GenericNode<ConstraintNode,ConstraintEdge>::GEdgeSetTy ConstraintEdgeSetTy;

    bool isPruned() {
        return pruned;
    }

    void setPruned() {
        pruned = true;
    }

};


/*!
 * Copy edge
 */
class AddrCGEdge: public ConstraintEdge {
private:
    AddrCGEdge();                      ///< place holder
    AddrCGEdge(const AddrCGEdge &);  ///< place holder
    void operator=(const AddrCGEdge &); ///< place holder
public:
    /// Methods for support type inquiry through isa, cast, and dyn_cast:
    //@{
    static inline bool classof(const AddrCGEdge *) {
        return true;
    }
    static inline bool classof(const ConstraintEdge *edge) {
        return edge->getEdgeKind() == Addr;
    }
    static inline bool classof(const GenericConsEdgeTy *edge) {
        return edge->getEdgeKind() == Addr;
    }
    //@}

    /// constructor
    AddrCGEdge(ConstraintNode* s, ConstraintNode* d, EdgeID id);
};


/*!
 * Copy edge
 */
class CopyCGEdge: public ConstraintEdge {
private:
    CopyCGEdge();                      ///< place holder
    CopyCGEdge(const CopyCGEdge &);  ///< place holder
    void operator=(const CopyCGEdge &); ///< place holder
public:
    /// Methods for support type inquiry through isa, cast, and dyn_cast:
    //@{
    static inline bool classof(const CopyCGEdge *) {
        return true;
    }
    static inline bool classof(const ConstraintEdge *edge) {
        return edge->getEdgeKind() == Copy;
    }
    static inline bool classof(const GenericConsEdgeTy *edge) {
        return edge->getEdgeKind() == Copy;
    }
    //@}

    /// constructor
    CopyCGEdge(ConstraintNode* s, ConstraintNode* d, EdgeID id) : ConstraintEdge(s,d,Copy,id) {
    }
};


/*!
 * Store edge
 */
class StoreCGEdge: public ConstraintEdge {
private:
    StoreCGEdge();                      ///< place holder
    StoreCGEdge(const StoreCGEdge &);  ///< place holder
    void operator=(const StoreCGEdge &); ///< place holder

public:
    /// Methods for support type inquiry through isa, cast, and dyn_cast:
    //@{
    static inline bool classof(const StoreCGEdge *) {
        return true;
    }
    static inline bool classof(const ConstraintEdge *edge) {
        return edge->getEdgeKind() == Store;
    }
    static inline bool classof(const GenericConsEdgeTy *edge) {
        return edge->getEdgeKind() == Store;
    }
    //@}

    /// constructor
    StoreCGEdge(ConstraintNode* s, ConstraintNode* d, EdgeID id) : ConstraintEdge(s,d,Store,id) {
    }
};


/*!
 * Load edge
 */
class LoadCGEdge: public ConstraintEdge {
private:
    LoadCGEdge();                      ///< place holder
    LoadCGEdge(const LoadCGEdge &);  ///< place holder
    void operator=(const LoadCGEdge &); ///< place holder

public:
    /// Methods for support type inquiry through isa, cast, and dyn_cast:
    //@{
    static inline bool classof(const LoadCGEdge *) {
        return true;
    }
    static inline bool classof(const ConstraintEdge *edge) {
        return edge->getEdgeKind() == Load;
    }
    static inline bool classof(const GenericConsEdgeTy *edge) {
        return edge->getEdgeKind() == Load;
    }
    //@}

    /// Constructor
    LoadCGEdge(ConstraintNode* s, ConstraintNode* d, EdgeID id) : ConstraintEdge(s,d,Load,id) {
    }
};

class ValFlowCGEdge: public ConstraintEdge {
    public:
        ValFlowCGEdge(ConstraintNode* s, ConstraintNode* d, ConstraintEdgeK k, EdgeID id) : ConstraintEdge(s, d, k, id) {
        }
};

/*!
 * Store Value edge
 */
class StoreValCGEdge: public ValFlowCGEdge {
private:
    StoreValCGEdge();                      ///< place holder
    StoreValCGEdge(const StoreValCGEdge &);  ///< place holder
    void operator=(const StoreValCGEdge &); ///< place holder
public:
    /// Methods for support type inquiry through isa, cast, and dyn_cast:
    //@{
    static inline bool classof(const StoreValCGEdge *) {
        return true;
    }
    static inline bool classof(const ConstraintEdge *edge) {
        return edge->getEdgeKind() == StoreVal;
    }
    static inline bool classof(const GenericConsEdgeTy *edge) {
        return edge->getEdgeKind() == StoreVal;
    }
    //@}
    /// constructor
    StoreValCGEdge(ConstraintNode* s, ConstraintNode* d, EdgeID id) : ValFlowCGEdge(s,d,StoreVal,id) {
    }
};

/*!
 * Load Value edge
 */
class LoadValCGEdge: public ValFlowCGEdge {
private:
    LoadValCGEdge();                      ///< place holder
    LoadValCGEdge(const LoadValCGEdge &);  ///< place holder
    void operator=(const LoadValCGEdge &); ///< place holder
public:
    /// Methods for support type inquiry through isa, cast, and dyn_cast:
    //@{
    static inline bool classof(const LoadValCGEdge *) {
        return true;
    }
    static inline bool classof(const ConstraintEdge *edge) {
        return edge->getEdgeKind() == LoadVal;
    }
    static inline bool classof(const GenericConsEdgeTy *edge) {
        return edge->getEdgeKind() == LoadVal;
    }
    //@}

    /// constructor
    LoadValCGEdge(ConstraintNode* s, ConstraintNode* d, EdgeID id) : ValFlowCGEdge(s,d,LoadVal,id) {
    }
};

/*!
 * Call Value edge
 */
class CallValCGEdge: public ValFlowCGEdge {
private:
    CallValCGEdge();                      ///< place holder
    CallValCGEdge(const CallValCGEdge &);  ///< place holder
    void operator=(const CallValCGEdge &); ///< place holder
public:
    /// Methods for support type inquiry through isa, cast, and dyn_cast:
    //@{
    static inline bool classof(const CallValCGEdge *) {
        return true;
    }
    static inline bool classof(const ConstraintEdge *edge) {
        return edge->getEdgeKind() == CallVal;
    }
    static inline bool classof(const GenericConsEdgeTy *edge) {
        return edge->getEdgeKind() == CallVal;
    }
    //@}

    /// constructor
    CallValCGEdge(ConstraintNode* s, ConstraintNode* d, EdgeID id) : ValFlowCGEdge(s,d,CallVal,id) {
    }
};

/*!
 * Return Value edge
 */
class RetValCGEdge: public ValFlowCGEdge {
private:
    RetValCGEdge();                      ///< place holder
    RetValCGEdge(const RetValCGEdge &);  ///< place holder
    void operator=(const RetValCGEdge &); ///< place holder
public:
    /// Methods for support type inquiry through isa, cast, and dyn_cast:
    //@{
    static inline bool classof(const RetValCGEdge *) {
        return true;
    }
    static inline bool classof(const ConstraintEdge *edge) {
        return edge->getEdgeKind() == RetVal;
    }
    static inline bool classof(const GenericConsEdgeTy *edge) {
        return edge->getEdgeKind() == RetVal;
    }
    //@}

    /// constructor
    RetValCGEdge(ConstraintNode* s, ConstraintNode* d, EdgeID id) : ValFlowCGEdge(s,d,RetVal,id) {
    }
};


/*!
 * Gep edge
 */
class GepCGEdge: public ConstraintEdge {
private:
    GepCGEdge();                      ///< place holder
    GepCGEdge(const GepCGEdge &);  ///< place holder
    void operator=(const GepCGEdge &); ///< place holder

protected:

    /// Constructor
    GepCGEdge(ConstraintNode* s, ConstraintNode* d, ConstraintEdgeK k, EdgeID id)
        : ConstraintEdge(s,d,k,id) {

    }

public:
    /// Methods for support type inquiry through isa, cast, and dyn_cast:
    //@{
    static inline bool classof(const GepCGEdge *) {
        return true;
    }
    static inline bool classof(const ConstraintEdge *edge) {
        return edge->getEdgeKind() == NormalGep ||
               edge->getEdgeKind() == VariantGep;
    }
    static inline bool classof(const GenericConsEdgeTy *edge) {
        return edge->getEdgeKind() == NormalGep ||
               edge->getEdgeKind() == VariantGep;
    }
    //@}

};

/*!
 * Gep edge with fixed offset size
 */
class NormalGepCGEdge : public GepCGEdge {
private:
    NormalGepCGEdge();                      ///< place holder
    NormalGepCGEdge(const NormalGepCGEdge &);  ///< place holder
    void operator=(const NormalGepCGEdge &); ///< place holder

    LocationSet ls;	///< location set of the gep edge

public:
    /// Methods for support type inquiry through isa, cast, and dyn_cast:
    //@{
    static inline bool classof(const NormalGepCGEdge *) {
        return true;
    }
    static inline bool classof(const GepCGEdge *edge) {
        return edge->getEdgeKind() == NormalGep;
    }
    static inline bool classof(const ConstraintEdge *edge) {
        return edge->getEdgeKind() == NormalGep;
    }
    static inline bool classof(const GenericConsEdgeTy *edge) {
        return edge->getEdgeKind() == NormalGep;
    }
    //@}

    /// Constructor
    NormalGepCGEdge(ConstraintNode* s, ConstraintNode* d, const LocationSet& l, EdgeID id)
        : GepCGEdge(s,d,NormalGep,id), ls(l)
    {}

    /// Get location set of the gep edge
    inline const LocationSet& getLocationSet() const {
        return ls;
    }
};

/*!
 * Gep edge with variant offset size
 */
class VariantGepCGEdge : public GepCGEdge {
private:
    VariantGepCGEdge();                      ///< place holder
    VariantGepCGEdge(const VariantGepCGEdge &);  ///< place holder
    void operator=(const VariantGepCGEdge &); ///< place holder

public:
    /// Methods for support type inquiry through isa, cast, and dyn_cast:
    //@{
    static inline bool classof(const VariantGepCGEdge *) {
        return true;
    }
    static inline bool classof(const GepCGEdge *edge) {
        return edge->getEdgeKind() == VariantGep;
    }
    static inline bool classof(const ConstraintEdge *edge) {
        return edge->getEdgeKind() == VariantGep;
    }
    static inline bool classof(const GenericConsEdgeTy *edge) {
        return edge->getEdgeKind() == VariantGep;
    }
    //@}

    /// Constructor
    VariantGepCGEdge(ConstraintNode* s, ConstraintNode* d, EdgeID id)
        : GepCGEdge(s,d,VariantGep,id)
    {}
};


#endif /* CONSGEDGE_H_ */
