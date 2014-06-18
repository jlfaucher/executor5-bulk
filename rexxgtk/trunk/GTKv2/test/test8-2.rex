#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007-2008 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/* Authors;                                                                   */
/*       W. David Ashley <dashley@us.ibm.com>                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/


-- Derived from Listing 8-2
-- Foundations of GTK+ Development
-- by Andrew Krause

call gtk_init
BUY_IT = 0
QUANTITY = 1
PRODUCT = 2
COLUMNS = 3

PRODUCT_CATEGORY = 0
PRODUCT_CHILD = 1

list = .array~new(1)
list[1] = .GroceryItem~new(PRODUCT_CATEGORY, .true, 0, 'Cleaning Supplies')
list[2] = .GroceryItem~new(PRODUCT_CHILD, .true, 1, 'Paper Towels')
list[3] = .GroceryItem~new(PRODUCT_CHILD, .true, 3, 'Toilet Paper')
list[4] = .GroceryItem~new(PRODUCT_CATEGORY, .true, 0, 'Food')
list[5] = .GroceryItem~new(PRODUCT_CHILD, .true, 2, 'Bread')
list[6] = .GroceryItem~new(PRODUCT_CHILD, .false, 1, 'Butter')
list[7] = .GroceryItem~new(PRODUCT_CHILD, .true, 1, 'Milk')
list[8] = .GroceryItem~new(PRODUCT_CHILD, .false, 3, 'Chips')
list[9] = .GroceryItem~new(PRODUCT_CHILD, .true, 4, 'Soda')

window = .myMainWindow~new(.gtk~GTK_WINDOW_TOPLEVEL)
window~set_title('Grocery List')
window~signal_connect('destroy')
window~set_border_width(10)
window~set_size_request(275, 300)

treeview = .GtkTreeview~new()
call setup_tree_view treeview, BUY_IT, QUANTITY, PRODUCT

store = .GtkTreeStore~new(.gtk~G_TYPE_BOOLEAN, .gtk~G_TYPE_INT, .gtk~G_TYPE_STRING)

do i = 1 to list~items
   if list[i]~product_type = PRODUCT_CATEGORY then do
      j = i + 1
      do while list[j]~product_type <> PRODUCT_CATEGORY
         if list[j]~buy then do
            list[i]~quantity += list[j]~quantity
            end
         j += 1
         if j > list~items then leave
         end
      iter = store~append()
      store~set_value(iter, BUY_IT, list[i]~buy, QUANTITY, list[i]~quantity,,
                      PRODUCT, list[i]~product)
      end
   else do
      child = store~append(iter)
      store~set_value(child, BUY_IT, list[i]~buy, QUANTITY, list[i]~quantity,,
                      PRODUCT, list[i]~product)
      end
   end

treeview~set_model(store)

treeview~set_model(store)
treeview~expand_all()

scrolled_win = .GtkScrolledWindow~new(.nil, .nil)
scrolled_win~set_policy(.gtk~GTK_POLICY_AUTOMATIC, .gtk~GTK_POLICY_AUTOMATIC)

scrolled_win~add(treeview)
window~add(scrolled_win)
window~show_all()

call gtk_main
return

::requires 'oorexxgtk2.cls'

::routine setup_tree_view
use strict arg treeview, BUY_IT, QUANTITY, PRODUCT

renderer = .GtkCellRendererText~new()
column = .GtkTreeViewColumn~newWithAttributes('Buy', renderer, 'text', BUY_IT)
treeview~append_column(column)

renderer = .GtkCellRendererText~new()
column = .GtkTreeViewColumn~newWithAttributes('Count', renderer, 'text', QUANTITY)
treeview~append_column(column)

renderer = .GtkCellRendererText~new()
column = .GtkTreeViewColumn~newWithAttributes('Product', renderer, 'text', PRODUCT)
treeview~append_column(column)
return

::class GroceryItem

::attribute product_type
::attribute buy
::attribute quantity
::attribute product

::method init
use strict arg self~product_type, self~buy, self~quantity, self~product
return

::class myMainWindow subclass GtkWindow

::method signal_destroy
call gtk_main_quit
return

