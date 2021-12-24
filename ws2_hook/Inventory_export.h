void Inventory_Init (void);
void Inventory_Insert (data32 inItem, data16 inRow, data16 inCol, data16 inQuantity);
void Inventory_ChangeQuantity (data16 inRow, data16 inCol, data16 inQuantity);
void Inventory_Decrease (data16 inRow, data16 inCol);
bool Inventory_Search (data32 inItem, data16 *inRow, data16 *inCol);
bool Inventory_QuickUseHP (data32 inItem, data16 inValue);
