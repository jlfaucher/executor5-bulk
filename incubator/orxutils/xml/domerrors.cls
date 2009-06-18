::class DOMErrors
::constant HIERARCHY_REQUEST_ERR  "An attempt was made to insert a node where it is not permitted."
::constant INDEX_SIZE_ERR         "The index or size is negative, or greater than the allowed value."
::constant INUSE_ATTRIBUTE_ERR    "An attempt is made to add an attribute that is already in use elsewhere."
::constant INVALID_ACCESS_ERR     "A parameter or an operation is not supported by the underlying object."
::constant INVALID_CHARACTER_ERR  "An invalid or illegal XML character is specified."
::constant INVALID_MODIFICATION_ERR "An attempt is made to modify the type of the underlying object."
::constant INVALID_STATE_ERR        "An attempt is made to use an object that is not, or is no longer, usable."
::constant NAMESPACE_ERR            "An attempt is made to create or change an object in a way which is incorrect with regard to namespaces."
::constant NOT_FOUND_ERR            "An attempt is made to reference a node in a context where it does not exist."
::constant NOT_SUPPORTED_ERR        "The implementation does not support the requested type of object or operation."
::constant NO_DATA_ALLOWED_ERR      "Data is specified for a node which does not support data."
::constant NO_MODIFICATION_ALLOWED_ERR "An attempt is made to modify an object where modifications are not allowed."
::constant SYNTAX_ERR                  "An invalid or illegal string is specified."
::constant VALIDATION_ERR              "A call to a method such as insertBefore or removeChild would make the Node invalid with respect to document grammar."
::constant WRONG_DOCUMENT_ERR          "A node is used in a different document than the one that created it."
::constant TYPE_MISMATCH_ERR           "The value type for this parameter name is incompatible with the expected value type."

::constant BAD_BOUNDARYPOINTS_ERR      "The boundary-points of a Range do not meet specific requirements."
::constant INVALID_NODE_TYPE_ERR       "The container of a boundary-point of a Range is being set to either a node of an invalid type or a node with an ancestor of an invalid type."

::method raiseError class
  use arg message

  raise syntax 98.900 array(message)    -- just raise this as a user execution error
