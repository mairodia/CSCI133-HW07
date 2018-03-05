// ============================================================================
// File: dll_remove_item.cpp (Spring 2018)
// ============================================================================
// Programmer: Jennifer King
// Project: Doubly Linked List - Remove Item
// Date: 3/4/2018
// Professor: Scott Edwards
// Class: CSCI 133F | TR 9:30AM
// Description:
//              This program allows the user to maintain a list of integer values that
//              are stored in a doubly-linked list. Values are stored in sorted
//              ascending order, and no duplicate values are allowed.
// ============================================================================

#include    <iostream>
#include    <cstdio>
#include    <cstdlib>
#include    <cctype>
using namespace std;


// structure definition
struct  LNode
{
    int         intVal;
    LNode       *prev;
    LNode       *next;
};


// function prototypes
void    DisplayList(LNode  *headPtr, LNode  *tailPtr);
void    DisplayMenu();
void    FlushInstream(istream  &inStream = cin);
bool    InsertItem(int  newValue, LNode  **headHandle, LNode  **tailHandle);
int     ReleaseList(LNode  **headHandle, LNode  **tailHandle);
bool    RemoveItem(int  target, LNode  **headHandle, LNode  **tailHandle);



// ==== main ==================================================================
//
// ============================================================================

int     main()
{
    bool        bLoop = true;
    int         intValue;
    LNode       *headPtr = NULL;
    LNode       *tailPtr = NULL;
    char        selection;

    // let the user manipulate the list
    do  {
        DisplayMenu();
        cout << "Please enter a selection: ";
        cin >> selection;
        switch (toupper(selection))
            {
            case   'A':
                cout << "Please enter a value to add to the list: ";
                if (cin >> intValue)
                    {
                    if (false == InsertItem(intValue, &headPtr, &tailPtr))
                        {
                        cout << "Error in inserting the new item into the list\n";
                        }
                    }
                else
                    {
                    cout << "Error reading integer, try again..." << endl;
                    FlushInstream();
                    }
                break;

            case   'R':
                // if the list is empty, just display a message
                if (NULL == headPtr)
                    {
                    cout << "Sorry, the list is already empty..." << endl << endl;
                    continue;
                    }

                // else, get a target value to remove from list
                cout << "Please enter a value to remove from the list: ";
                if (cin >> intValue)
                    {
                    if (false == RemoveItem(intValue, &headPtr, &tailPtr))
                        {
                        cout << "Error in removing the item from the list\n";
                        }
                    }
                else
                    {
                    cout << "Error reading integer, try again..." << endl;
                    FlushInstream();
                    }
                break;

            case   'D':
                if (NULL == headPtr)
                    {
                    cout << "The list is currently empty..." << endl << endl;
                    }
                else
                    {
                    cout << "Here is the current list:\n";
                    DisplayList(headPtr, tailPtr);
                    cout << endl;
                    }
                break;

            case   'Q':
                bLoop = false;
                cout << "Number of heap nodes released: "
                     << ReleaseList(&headPtr, &tailPtr)
                     << endl;
                cout << "Bye!" << endl;
                break;

            default:
                cout << "Unrecognized selection; please try again...\n\n";
                FlushInstream();
                break;
            }

        cout << endl;

        } while (true == bLoop);

    return 0;

}  // end of "main"



// ==== DisplayMenu ===========================================================
//
// This function simply displays the list of menu options available to the
// user.
//
// Input:  nothing
// Output: nothing
//
// ============================================================================

void    DisplayMenu()
{
    cout << "A)dd an item\n";
    cout << "R)emove an item\n";
    cout << "D)isplay the list\n";
    cout << "Q)uit\n";

}  // end of "DisplayMenu"



// ==== DisplayList ===========================================================
//
// This function displays the contents of the doubly-linked list.  It uses the
// pointer arguments to display the list from front to back and from back to
// front.
//
// Input:
//      headPtr    -- a pointer to the first node in the list
//
//      tailPtr    -- a pointer to the last node in the list
//
// Output:
//      Nothing.
//
// ============================================================================

void    DisplayList(LNode  *headPtr, LNode  *tailPtr)
{
    cout << "*** From front to back..." << endl;
    while(headPtr != NULL)
    {
        cout << headPtr -> intVal << endl;
        headPtr = headPtr -> next;
    }

    cout << "*** From back to front..." << endl;
    while(tailPtr != NULL)
    {
        cout << tailPtr -> intVal << endl;
        tailPtr = tailPtr -> prev;
    }

}  // end of "DisplayList"



// ==== InsertItem ============================================================
//
// This function allows the user to insert a new value into the doubly-linked
// list.  If the value already exists in the list, this is flagged as an error
// and a value of "false" is returned.  Otherwise, a new node is allocated and
// initialized with the new value, and the new node is inserted into the list.
// The caller's head and tail pointers will be updated if the new node is
// inserted to the very front of very end of the list. The list is maintained
// in sorted ascending order.
//
// Input:
//      newValue        -- the value of the item to insert into the doubly
//                         linked list
//
//      headHandle      -- a pointer to a pointer to the first node in the list
//
//      tailHandle      -- a pointer to a pointer to the last node in the list
//
// Output:
//      A value of "true" if the item is successfully added to the list,
//      "false" if not.  (Note -- attempts to insert duplicate values will
//      cause a value of "false" to be returned.)
//
// ============================================================================

bool    InsertItem(int  newValue, LNode  **headHandle, LNode  **tailHandle)
{
    LNode       *nodePtr;
    LNode       *newItem;

    // allocate a node for the new value
    newItem = new LNode;
    newItem->prev = NULL;
    newItem->next = NULL;
    newItem->intVal = newValue;

    // if the list is empty, all we have to do is update the caller's head and
    // tail pointers and return
    if (NULL == *headHandle)
        {
        *headHandle = *tailHandle = newItem;
        return true;
        }

    // loop until the new item value is less than the current value in the list
    nodePtr = *headHandle;
    while ((newValue > nodePtr->intVal)  &&  (*tailHandle != nodePtr))
        {
        nodePtr = nodePtr->next;
        }

    // if a duplicate item was found, release allocated memory and return
    if (newValue == nodePtr->intVal)
        {
        delete  newItem;
        return false;
        }

    // insert the new value into the list
    if (newValue < nodePtr->intVal)
        {
        if (NULL != nodePtr->prev)
            {
            // inserting before the breakpoint node but not at head, so connect
            // the previous node to the new node
            nodePtr->prev->next = newItem;
            newItem->prev = nodePtr->prev;
            }

        // connect the new node to the node pointed to by the list pointer
        newItem->next = nodePtr;
        nodePtr->prev = newItem;

        // adjust head pointer if necessary
        if (nodePtr == *headHandle)
            {
            *headHandle = newItem;
            }
        }
    else    // newValue > nodePtr->intVal
        {
            // inserting after the breakpoint node
        nodePtr->next = newItem;
        newItem->prev = nodePtr;
        *tailHandle = newItem;
        }

    return true;

}  // end of "InsertItem"



// ==== RemoveItem ============================================================
//
// This function removes a target item from the doubly-linked list.  If either
// the head or tail pointers to the list are involved, their values are updated
// so the caller can rely upon their values after this function returns.
//
// Input:
//      target          -- the value of the item to remove from the doubly
//                         linked list
//
//      headHandle      -- a pointer to a pointer to the first node in the list
//
//      tailHandle      -- a pointer to a pointer to the last node in the list
//
// Output:
//      A value of "true" if the item is found and removed from the list,
//      "false" if not.
//
// ============================================================================

bool    RemoveItem(int  target, LNode  **headHandle, LNode  **tailHandle)
{
    if(*headHandle == NULL) // if list is empty
    {
        return false;
    }

    LNode *current = *headHandle;
    LNode *next;
    LNode *del;

    while(current != NULL)
    {
        if(current -> intVal == target)
        {
            next = current -> next;
            del = current;
            if(*headHandle == del) // if node delete is head
            {
                *headHandle = del -> next;
            }
            if(del -> next != NULL) // only change next if del is NOT tail
            {
                del -> next -> prev = del -> prev;
            }
            if(del -> prev != NULL) // only change prev if del is NOT head
            {
                del -> prev -> next = del -> next;
            }
            delete [] del;
            current = next;
        }
        else
        {
            current = current -> next;
        }
    }

    return true;
}  // end of "RemoveItem"



// ==== ReleaseList ===========================================================
//
// This function receives pointers to the caller's head and tail pointers. All
// of the nodes in the list are released back to the heap, the caller's head
// and tail pointers are set to NULL, and the total number of nodes released
// is returned to the caller.
// Input:
//      headHandle      -- a pointer to a pointer to the first node in the list
//
//
//      tailHandle      -- a pointer to a pointer to the last node in the list
//
// Output:
//      An integer value representing the number of list nodes released back
//      to the heap.
//
// ============================================================================

int     ReleaseList(LNode  **headHandle, LNode  **tailHandle)
{
    LNode *current = *headHandle;
    LNode *next;
    int counter = 0;

    while(current != NULL)
    {
        next = current -> next;
        current = next;
        delete []current;
        counter ++;
    }
    return counter;

}  // end of "ReleaseList"



// ==== FlushInstream =========================================================
//
// This function displays the current contents of the list to the standard
//
// Input:
//      list    -- a const reference to a CSortedList object
//
// Output:
//      Nothing.
//
// ============================================================================

void    FlushInstream(istream  &inStream)
{
    char        inChar;

    inStream.clear();
    while (false == inStream.eof())
        {
        inStream.get(inChar);
        if ('\n' == inChar)
            {
            break;
            }
        }

}  // end of "FlushInstream"
