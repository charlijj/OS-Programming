/* Jasper Charlinski
   658366497

   This file provides the functions for various opperations regarding a linked list of nodes
*/

#include "KV.h"
#include <iostream>

void initialize(KVlist &L)
{
   L.front = NULL;
   L.back = NULL;
   L.nnodes = 0; 
}


void deallocate(KVlist &L)
{
   KVnode * current = L.front;
   KVnode * toDelete;

   while(current != NULL)
   {
      toDelete = current;
      current = current->next;
      delete toDelete;
   }
}


int size(KVlist L)
{
   return L.nnodes; 
}


void print(KVlist L)
{
   KVnode * current = L.front;

   while(current != NULL)
   {
      printData(current->key, current->desc, current->value);

      current = current->next;
   }
}


bool insert(KVlist &L, long k, std::string d, double v)
{
   KVnode * current = create(k, d, v);

   if (current == NULL)
   {
      return false;
   }

   // If the list is empty
   if (L.front == NULL)
   {
      L.front = current;
      L.back = current;
   }
   // If the new node is to be inserted into the front
   else if (current->key < L.front->key)
   {
      L.front->prev = current;
      current->next = L.front;
      L.front = current;
   }
   // If the new node is to be inserted into the back
   else if (current->key > L.back->key)
   {
      L.back->next = current;
      current->prev = L.back;
      L.back = current;
   }
   // if the new node is to be inserted in between two other nodes
   else
   {
      KVnode * after = L.front;

      while(after->key <= current->key)
      {
         // If the key already exists in the list
         if (after->key == current->key)
         {
            // Update the description and value
            after->desc = current->desc;
            after->value = current->value;
            return true;
         }
         after = after->next;
      }
      KVnode * before = after->prev;

      before->next = current;
      after->prev = current;
      current->next = after;
      current->prev = before;
   }
   
   return true;
}


bool remove(KVlist &L, long k, std::string &d, double &v)
{
   KVnode * toRemove = lookup(L, k);

   // If the node was not found in the list
   if(toRemove == NULL)
   {
      return false;
   }

   // Retreive the node description and value before node is removed
   d = toRemove->desc;
   v = toRemove->value;

   // If there is only one node in the list
   if((toRemove->next == NULL) && (toRemove->prev == NULL))
   {
      // Reset list to its original state
      initialize(L);
      return true;
   }
   // If the node to remove is in the back of the list
   else if (toRemove->next == NULL)
   {
      L.back = toRemove->prev;
      L.back->next = NULL;
   }
   // If the node to remove is in the front of the list
   else if (toRemove->prev == NULL)
   {
      L.front = toRemove->next;
      L.front->prev = NULL;
   }
   // If the node is in between two other nodes
   else
   {
      KVnode * before = toRemove->prev;
      KVnode * after = toRemove->next;
      before->next = after;
      after->prev = before;
   }
   // Subtract one from node count
   L.nnodes--;
   return true;

   delete toRemove;
}


KVnode* lookup(KVlist L, long k)
{
   KVnode * current = L.front;

   while(current != NULL)
   {
      if (current->key == k)
      {
         return current;
      }
      current = current->next;
   }
   // If the requested key is not found in the list
   return NULL;
}


bool lookup(KVlist L, long k, std::string &d, double &v)
{
   KVnode * inList = lookup(L, k);

   // If the node was not found in the list
   if(inList == NULL)
   {
      return  false;
   }
   else
   {
      d = inList->desc;
      v = inList->value;
      return true;
   }
}
