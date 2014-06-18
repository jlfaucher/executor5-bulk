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


-- Derived from Listing 8-6
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
treeview~expand_all()

scrolled_win = .GtkScrolledWindow~new(.nil, .nil)
scrolled_win~set_policy(.gtk~GTK_POLICY_AUTOMATIC, .gtk~GTK_POLICY_AUTOMATIC)

selection = treeview~get_selection()
selection~set_mode(.gtk~GTK_SELECTION_MULTIPLE)

scrolled_win~add(treeview)
scrolled_win~set_policy(.gtk~GTK_POLICY_AUTOMATIC, .gtk~GTK_POLICY_AUTOMATIC)

add = .MyAddButton~newFromStock(.gtk~GTK_STOCK_ADD)
remove = .MyRemoveButton~newFromStock(.gtk~GTK_STOCK_REMOVE)

add~user_data = .array~new()
remove~user_data = .array~new()
add~user_data[1] = treeview
add~user_data[2] = list
add~user_data[3] = PRODUCT_CATEGORY
add~user_data[4] = BUY_IT
add~user_data[5] = PRODUCT
add~user_data[6] = QUANTITY
add~user_data[7] = store
remove~user_data[1] = treeview

add~signal_connect('clicked')
remove~signal_connect('clicked')

hbox = .GtkHBox~new(.true, 5)
hbox~pack_start(add, .false, .true, 0)
hbox~pack_start(remove, .false, .true, 0)

vbox = .GtkVBox~new(.false, 5)
vbox~pack_start(scrolled_win, .true, .true, 0)
vbox~pack_start(hbox, .false, .true, 0)

window~add(vbox)
window~show_all()

call gtk_main
return

::requires 'rexxgtk.cls'

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

::class MyAddButton subclass GtkButton

::method signal_clicked
treeview = self~user_data[1]
list = self~user_data[2]
PRODUCT_CATEGORY = self~user_data[3]
BUY_IT = self~user_data[4]
PRODUCT = self~user_data[5]
QUANTITY = self~user_data[6]
store = self~user_data[7]

dialog = .GtkDialog~new('Add a Product', .nil, .gtk~GTK_DIALOG_MODAL,,
                        .gtk~GTK_STOCK_ADD, .gtk~GTK_RESPONSE_OK,,
                        .gtk~GTK_STOCK_CANCEL, .gtk~GTK_RESPONSE_CANCEL)
combobox = .GtkComboBoxText~new()
entry = .GtkEntry~new();
spin = .GtkSpinButton~newWithRange(0, 100, 1)
check = .GtkCheckButton~newWithMnemonic('_Buy the Product')
spin~set_digits(0)

do prod over list
   if prod~product_type = PRODUCT_CATEGORY then do
      combobox~append_text(prod~product)
      end
   end

table = .GtkTable~new(4, 2, .false)
table~set_row_spacings(5)
table~set_col_spacings(5)
table~set_border_width(5)

table~attach(.GtkLabel~new('Category:'), 0, 1, 0, 1,,
             .gtk~GTK_SHRINK + .gtk~GTK_FILL, .gtk~GTK_SHRINK + .gtk~GTK_FILL,,
             0, 0)
table~attach(combobox, 1, 2, 0, 1,,
             .gtk~GTK_EXPAND + .gtk~GTK_FILL, .gtk~GTK_SHRINK + .gtk~GTK_FILL,,
             0, 0)
table~attach(.GtkLabel~new('Product:'), 0, 1, 1, 2,,
             .gtk~GTK_SHRINK + .gtk~GTK_FILL, .gtk~GTK_SHRINK + .gtk~GTK_FILL,,
             0, 0)
table~attach(entry, 1, 2, 1, 2,,
             .gtk~GTK_EXPAND + .gtk~GTK_FILL, .gtk~GTK_SHRINK + .gtk~GTK_FILL,,
             0, 0)
table~attach(.GtkLabel~new('Quantity:'), 0, 1, 2, 3,,
             .gtk~GTK_SHRINK + .gtk~GTK_FILL, .gtk~GTK_SHRINK + .gtk~GTK_FILL,,
             0, 0)
table~attach(spin, 1, 2, 2, 3,,
             .gtk~GTK_EXPAND + .gtk~GTK_FILL, .gtk~GTK_SHRINK + .gtk~GTK_FILL,,
             0, 0)
table~attach(check, 1, 2, 3, 4,,
             .gtk~GTK_EXPAND + .gtk~GTK_FILL, .gtk~GTK_SHRINK + .gtk~GTK_FILL,,
             0, 0)

dialog~vbox~pack_start_defaults(table)
dialog~show_all()

if dialog~dialog_run() = .gtk~GTK_RESPONSE_OK then do
   quant = spin~get_text()
   prod = entry~get_text()
   cat = combobox~get_active_text()
   buy = check~get_active()

   if prod = '' | cat = '' then do
      say 'All of the fields were not correctly filled out!'
      dialog~destroy()
      return
      end

   model = treeview~get_model()
   iter = model~get_iter_from_string('0');

   do while \iter~isNull()
      name = model~get_value(iter, PRODUCT)
      if name = cat then leave
      iter = model~iter_next(iter)
      end

   path = model~get_path(iter)
   child = store~append(iter)
   store~set_value(child, BUY_IT, buy, QUANTITY, quant, PRODUCT, prod)

   if buy = .true then do
      path = model~get_path(iter)
      i = model~get_value(iter, QUANTITY)
      i += quant
      store~set_value(iter, QUANTITY, i)
      end
   end

dialog~destroy()
return

::class MyRemoveButton subclass GtkButton

::method signal_clicked
return






