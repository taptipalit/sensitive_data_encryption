//===- WorkList.h -- Internal worklist used in SVF---------------------------//
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
 * @file: WorkList.h
 * @author: yesen
 * @date: 06/12/2013
 * @version: 1.0
 *
 * @section LICENSE
 *
 * @section DESCRIPTION
 *
 */


#ifndef WORKLIST_H_
#define WORKLIST_H_

#include <assert.h>
#include <cstdlib>
#include <vector>
#include <deque>
#include <set>

/**
 * Worlist with "first come first go" order.
 * New nodes pushed at back and popped from front.
 * Elements in the list are unique as they're recorded by std::set.
 */
template<class Data>
class List {
    class ListNode {
    public:
        ListNode(Data d) {
            data = d;
            next = NULL;
        }

        ~ListNode() {}

        Data data;
        ListNode* next;
    };

    typedef std::set<Data> DataSet;
    typedef ListNode Node;

public:
    List() {
        head = NULL;
        tail = NULL;
    }

    ~List() {}

    inline bool empty() const {
        return (head == NULL);
    }

    inline bool find(Data data) const {
        return (nodeSet.find(data) == nodeSet.end() ? false : true);
    }

    void push(Data data) {
        if (nodeSet.find(data) == nodeSet.end()) {
            Node* new_node = new Node(data);
            if (head == NULL)
                head = new_node;// the list is empty
            else
                tail->next = new_node;
            tail = new_node;
        }
    }

    Data pop() {
        assert(head != NULL && "list is empty");
        /// get node from list head
        Node* head_node = head;

        /// change list head to the next node
        head = head->next;
        if (head == NULL)
            tail = NULL;	/// the last node is popped.

        Data data = head_node->data;
        nodeSet.erase(data);
        delete head_node;
        return data;
    }

private:
    DataSet nodeSet;
    Node* head;
    Node* tail;
};

/**
 * Worlist with "first in first out" order.
 * New nodes will be pushed at back and popped from front.
 * Elements in the list are unique as they're recorded by std::set.
 */
template<class Data>
class FIFOWorkList {
    typedef std::set<Data> DataSet;
    typedef std::deque<Data> DataDeque;
public:
    FIFOWorkList() {}

    ~FIFOWorkList() {}

    inline bool empty() const {
        return data_list.empty();
    }

    inline bool find(Data data) const {
        return (data_set.find(data) == data_set.end() ? false : true);
    }

    /**
     * Push a data into the work list.
     */
    inline bool push(Data data) {
        if (data_set.find(data) == data_set.end()) {
            data_list.push_back(data);
            data_set.insert(data);
            return true;
        }
        else
            return false;
    }

    /**
     * Pop a data from the END of work list.
     */
    inline Data pop() {
        assert(!empty() && "work list is empty");
        Data data = data_list.front();
        data_list.pop_front();
        data_set.erase(data);
        return data;
    }

    /*!
     * Clear all the data
     */
    inline void clear() {
        data_list.clear();
        data_set.clear();
    }

    inline int size() {
        return data_set.size();
    }

    inline static void copyWorkList(FIFOWorkList& src, FIFOWorkList& dst) {
        typename std::set<Data>::iterator it;
        for (it = src.data_set.begin(); it != src.data_set.end(); it++) {
            dst.push(*it);
        }
    }

private:
    DataSet data_set;	///< store all data in the work list.
    DataDeque data_list;	///< work list using std::vector.
};

/**
 * Worlist with "first in last out" order.
 * New nodes will be pushed at back and popped from back.
 * Elements in the list are unique as they're recorded by std::set.
 */
template<class Data>
class FILOWorkList {
    typedef std::set<Data> DataSet;
    typedef std::vector<Data> DataVector;
public:
    FILOWorkList() {}

    ~FILOWorkList() {}

    inline bool empty() const {
        return data_list.empty();
    }

    inline bool find(Data data) const {
        return (data_set.find(data) == data_set.end() ? false : true);
    }

    /**
     * Push a data into the work list.
     */
    inline bool push(Data data) {
        if (data_set.find(data) == data_set.end()) {
            data_list.push_back(data);
            data_set.insert(data);
            return true;
        }
        else
            return false;
    }

    /**
     * Pop a data from the END of work list.
     */
    inline Data pop() {
        assert(!empty() && "work list is empty");
        Data data = data_list.back();
        data_list.pop_back();
        data_set.erase(data);
        return data;
    }

    /*!
     * Clear all the data
     */
    inline void clear() {
        data_list.clear();
        data_set.clear();
    }

private:
    DataSet data_set;	///< store all data in the work list.
    DataVector data_list;	///< work list using std::vector.
};



#endif /* WORKLIST_H_ */
