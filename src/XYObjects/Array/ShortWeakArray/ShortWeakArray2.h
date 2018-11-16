#include "../../../XYResult.h"
#include "../../XYObject.h"
#include "../../XYObjectTypes.h"


/**
 * @brief Add Item to a Short Weak Array
 * 
 * @param self 
 * @param newItemType 
 * @param length 
 * @return XYResult_t 
 */
XYResult_t ShortWeakArray_add(XYObject_t *self,
                              unsigned char newItemType[2],
                              int length);

/**
 * @brief Get Item from a Short Weak Array
 * 
 * @param self 
 * @param index 
 * @return XYResult_t 
 */
XYResult_t ShortWeakArray_get(XYObject_t *self,
                              int index);
