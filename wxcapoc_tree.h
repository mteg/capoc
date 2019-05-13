
class wxCapoc_ItemData : public wxTreeItemData {
public:
    explicit wxCapoc_ItemData(unsigned int type);
    unsigned int itemType;

    std::string pointName = "";
    union {
        unsigned int modelId = 0;
        unsigned int camIndex;
    };
};

enum {
    ITEM_ROOT = 0,
    ITEM_MODEL, ITEM_POINT, ITEM_IMAGE
};

class wxCapoc_Tree : public wxTreeCtrl {
public:
    explicit wxCapoc_Tree(wxWindow *parent, capEngine *engine);
    capEngine *engine;

    wxTreeItemId modelsRoot, imagesRoot;
    int modelEditing = -1;

    void resync();
    void resync_models();
    void resync_nvm();

private:
    void ensureChildrenCount(wxTreeItemId& id, unsigned int cnt, unsigned int itemType = 0);
    void OnMenu( wxTreeEvent& event);
    void OnSelect( wxTreeEvent& event);
    void OnActivate( wxTreeEvent& event);
    void OnAddPoint( wxCommandEvent& event);
    void OnEditPoint( wxCommandEvent& event);
    void OnDeletePoint( wxCommandEvent& event);
    void QuickMenuAction( wxCommandEvent& event);
    wxCapoc_ItemData *procItem = NULL;
    unsigned int masterModel = 0;


wxDECLARE_EVENT_TABLE();

    void showLog(const char *log, const char *title);
};


enum {
    ID_Add_Point = 1,
    ID_Delete_Point,
    ID_Edit_Point,
    ID_Display_None, ID_Display_Points, ID_Display_Faces, ID_Display_Edges,
    ID_Selection_FitPlane, ID_Selection_Save, ID_Selection_Reset, ID_Selection_Delete,
    ID_Info, ID_Append_Data, ID_Replace_Data, ID_Save, ID_Compute_Normals, ID_Set_Gray,
    ID_Transform_Matrix, ID_Transform_Reset, ID_Transform_To, ID_Set_Master, ID_Resync,
    ID_Sort_Name, ID_Sort_CamIndex, ID_Sort_Seen, ID_Sort_Depth, ID_Sort_Distance,
    ID_Revert_Data, ID_Set_Edit, ID_Cancel_Edit, ID_NVM_Info,
    ID_Cam_See, ID_Cam_Select_Projectable, ID_Cam_Recolor_Projectable, ID_Cam_Delete_Unprojectable, ID_Cam_Gray_Unprojectable,
};

