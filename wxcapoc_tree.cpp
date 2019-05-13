/*
 *
 * Copyright (c) 2019 Mateusz 'mteg' Golicz
 *
 * Distributed under Apache License version 2.0
 *
 */
#include "capengine.h"
#include "caprenderer_ancientgl.h"
#include "capdebug.h"
#include "wxcapoc.h"
#include "wxcapoc_editpoint.h"
#include "wxcapoc_editmatrix.h"
#include "wxcapoc_logbrowser.h"

wxBEGIN_EVENT_TABLE(wxCapoc_Tree, wxTreeCtrl)
                EVT_TREE_ITEM_MENU(wxID_ANY, wxCapoc_Tree::OnMenu)
                EVT_TREE_SEL_CHANGING(wxID_ANY, wxCapoc_Tree::OnSelect)
                EVT_TREE_ITEM_ACTIVATED(wxID_ANY, wxCapoc_Tree::OnActivate)
                EVT_MENU(ID_Add_Point, wxCapoc_Tree::OnAddPoint)
                EVT_MENU(ID_Edit_Point, wxCapoc_Tree::OnEditPoint)
                EVT_MENU(ID_Delete_Point, wxCapoc_Tree::OnDeletePoint)
                EVT_MENU(wxID_ANY, wxCapoc_Tree::QuickMenuAction)
wxEND_EVENT_TABLE()

wxCapoc_Tree::wxCapoc_Tree(wxWindow *parent, capEngine *engine) :
    wxTreeCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxTR_HIDE_ROOT)
{
    this->engine = engine;
    wxTreeItemId root = AddRoot(wxT("<irrelevant>"), -1, -1, new wxCapoc_ItemData(ITEM_ROOT));
    modelsRoot = AppendItem(root, wxT("Models"), -1, -1, new wxCapoc_ItemData(ITEM_ROOT));
    imagesRoot = AppendItem(root, wxT("Images"), -1, -1, new wxCapoc_ItemData(ITEM_ROOT));
}

void wxCapoc_Tree::resync() {
    resync_models();
    resync_nvm();
}

void wxCapoc_Tree::resync_nvm() {
    unsigned int n = 0;
    capNvm *nvm = NULL;

    if(!engine->models.empty())
        if(engine->models[0]->nvm)
        {
            nvm = engine->models[0]->nvm;
            n = nvm->n_cams;
        }

    /* Make sure there is an identical number of models in the tree and in the engine */
    ensureChildrenCount(imagesRoot, n, ITEM_IMAGE);
    if(!n) return;

    /* Update model names */
    wxTreeItemIdValue c_cookie;
    wxTreeItemId m = GetFirstChild(imagesRoot, c_cookie);

    /* Resync models */
    unsigned int i;
    for(i = 0; m.IsOk(); i++) {
        capNvmCam &ca = nvm->cams[nvm->camIndex[i]];

        /* Resync cam name */
        if (ca.seenFeatures == 0)
            SetItemText(m, wxString::FromUTF8(ca.filename));
        else {
            wxString label, filename = wxString::FromUTF8(ca.filename);

            label.Printf(wxT("%s (%d)"), filename, ca.seenFeatures);
            SetItemText(m, label);
        }


        /* Resync cam ID */
        auto *md = (wxCapoc_ItemData*) GetItemData(m);
        md->camIndex = nvm->camIndex[i];

        /* Go to next model */
        m = GetNextChild(imagesRoot, c_cookie);
    }

}

void wxCapoc_Tree::resync_models() {
    /* Make sure there is an identical number of models in the tree and in the engine */
    ensureChildrenCount(modelsRoot, (unsigned int) engine->models.size(), ITEM_MODEL);

    /* Update model names */
    wxTreeItemIdValue m_cookie;
    wxTreeItemId m = GetFirstChild(modelsRoot, m_cookie);

    /* Resync models */
    unsigned int i;
    for(i = 0; m.IsOk(); i++)
    {
        /* Resync model name */

        if (modelEditing != ((int) i)) {
            printf("item text: %s\n", engine->models[i]->filename);
            SetItemText(m, wxString::FromUTF8(engine->models[i]->filename));
        }
        else {
            wxString label, filename = wxString::FromUTF8(engine->models[i]->filename);
            label.Printf(wxT("%s (editing)"), filename);
            SetItemText(m, label);
        }

        /* Resync model master state */
        SetItemBold(m, i == masterModel);

        /* Resync model ID */
        wxCapoc_ItemData *md = (wxCapoc_ItemData*) GetItemData(m);
        md->modelId = i;

        /* Resync saved points */
        ensureChildrenCount(m, (unsigned int) engine->models[i]->savedPoints.size(), ITEM_POINT);
        wxTreeItemIdValue p_cookie;
        wxTreeItemId p = GetFirstChild(m, p_cookie);
        for(auto const & it: engine->models[i]->savedPoints)
        {
            SetItemText(p, wxString::FromUTF8(it.first.c_str()));

            wxCapoc_ItemData *pd = (wxCapoc_ItemData*) GetItemData(p);
            pd->modelId = i;
            pd->pointName = it.first;

            p = GetNextChild(m, p_cookie);
        }

        /* Go to next model */
        m = GetNextChild(modelsRoot, m_cookie);
    }
}

void wxCapoc_Tree::ensureChildrenCount(wxTreeItemId& parent, unsigned int c, unsigned int itemType)
{
    while(GetChildrenCount(parent, false) > c) {
        Delete(GetLastChild(parent));
    }

    while(GetChildrenCount(parent, false) < c) {
        AppendItem(parent, "<to be updated>", -1, -1, new wxCapoc_ItemData(itemType));
    }
}

void wxCapoc_Tree::OnMenu(wxTreeEvent &event) {
    auto* menu = new wxMenu;
    procItem =  (wxCapoc_ItemData*) GetItemData(event.GetItem());

    switch(procItem->itemType)
    {
        case ITEM_ROOT:
        {
            menu->Append(ID_Resync, wxT("Resync tree"));
            if(event.GetItem().GetID() == imagesRoot)
            {
                auto *orderMenu = new wxMenu;
                orderMenu->Append(ID_Sort_Name, wxT("Filename"), wxT("Order images by file name"));
                orderMenu->Append(ID_Sort_CamIndex, wxT("Index"), wxT("Revert to original order from NVM file"));
                orderMenu->Append(ID_Sort_Depth, wxT("Depth"), wxT("Order images by depth relative to camera"));
                orderMenu->Append(ID_Sort_Distance, wxT("Distance"), wxT("Find images closest to camera"));
                orderMenu->Append(ID_Sort_Seen, wxT("Seen features"), wxT("Order images by seen features"));

                menu->Append(wxID_ANY, wxT("Order by"), orderMenu);
            }
            break;
        }
        case ITEM_MODEL:
            {
            auto *displayMenu = new wxMenu;
            displayMenu->Append(ID_Display_None, wxT("None"));
            displayMenu->Append(ID_Display_Points, wxT("Points"));
            displayMenu->Append(ID_Display_Edges, wxT("Edges"));
            displayMenu->Append(ID_Display_Faces, wxT("Faces"));

            auto *selectionMenu = new wxMenu;
            selectionMenu->Append(ID_Selection_FitPlane, wxT("Fit plane"));
            selectionMenu->Append(ID_Selection_Save, wxT("Save as..."));
            selectionMenu->Append(ID_Selection_Reset, wxT("Reset"));
            selectionMenu->Append(ID_Selection_Delete, wxT("Delete"));

            auto *transformMenu = new wxMenu;
            transformMenu->Append(ID_Transform_Matrix, wxT("Transformation matrix..."));
            transformMenu->Append(ID_Transform_To, wxT("Transform to master"));
            transformMenu->Append(ID_Transform_Reset, wxT("Reset"));

            if(((int) procItem->modelId) == modelEditing)
                transformMenu->Append(ID_Cancel_Edit, wxT("Stop editing"));
            else
                transformMenu->Append(ID_Set_Edit, wxT("Edit with mouse"));

            menu->Append(ID_Set_Master, wxT("Set as master"));
            menu->Append(wxID_ANY, wxT("Display"), displayMenu);
            menu->Append(wxID_ANY, wxT("Selection"), selectionMenu);
            menu->Append(wxID_ANY, wxT("Transform"), transformMenu);
            menu->Append(ID_Info, wxT("Information..."));
            menu->Append(ID_Add_Point, wxT("Add point..."));
            menu->Append(ID_Append_Data, wxT("Append data..."));
            menu->Append(ID_Replace_Data, wxT("Replace data..."));
            menu->Append(ID_Save, wxT("Save as..."));
            menu->Append(ID_Compute_Normals, wxT("Compute normals"));
            menu->Append(ID_Set_Gray, wxT("Set to gray"));
            menu->Append(ID_Revert_Data, wxT("Revert"), wxT("Reload the source file"));
            break;
        }
        case ITEM_POINT:
            menu->Append(ID_Edit_Point, wxT("Edit point..."));
            menu->Append(ID_Delete_Point, wxT("Delete point"));
            break;
        case ITEM_IMAGE:
            menu->Append(ID_Cam_See, wxT("Assume pose"), wxT("Set caver camera to wherever this picture was taken"));
            menu->Append(ID_Cam_Select_Projectable, wxT("Select seen"), wxT("Select model points seen by this camera"));
            menu->Append(ID_Cam_Recolor_Projectable, wxT("Recolor seen"), wxT("Set model color from this camera"));
            menu->Append(ID_Cam_Delete_Unprojectable, wxT("Delete invisible"), wxT("Delete points not seen by this camera"));
            menu->Append(ID_Cam_Gray_Unprojectable, wxT("Set invisible to gray"), wxT("Set points not seen by this camera to gray"));
            menu->Append(ID_NVM_Info, wxT("Information..."), wxT("Show information on this NVM camera"));
            break;

    }
    PopupMenu(menu, wxDefaultPosition);
    delete menu;
}

void wxCapoc_Tree::OnAddPoint(wxCommandEvent &event) {

    wxCapoc_EditPoint dialog(engine->models[procItem->modelId]->cursor, wxT("Add new point"));
    dialog.TransferDataToWindow();
    if(dialog.ShowModal() == wxID_OK) {
        wxCharBuffer chb = dialog.nameInput->GetValue().ToUTF8();
        char *const nowName = chb.data();
        dialog.TransferDataFromWindow();
        engine->models[procItem->modelId]->setPoint(nowName, dialog.p);
    }

    dialog.Destroy();
    resync_models();
}

void wxCapoc_Tree::OnEditPoint(wxCommandEvent &event) {
    char *const prevName = (char* const) procItem->pointName.c_str();
    wxCapoc_EditPoint dialog(engine->models[procItem->modelId]->savedPoints[prevName], wxT("Edit point"), procItem->pointName);
    if(dialog.ShowModal() == wxID_OK) {
        wxCharBuffer chb = dialog.nameInput->GetValue().ToUTF8();
        char *const nowName = chb.data();
        engine->models[procItem->modelId]->deletePoint(prevName);
        engine->models[procItem->modelId]->setPoint(nowName, dialog.p);
    }

    dialog.Destroy();
    resync();

}

void wxCapoc_Tree::OnDeletePoint(wxCommandEvent &event) {
    char *const pName = (char *const) procItem->pointName.c_str();
    engine->models[procItem->modelId]->deletePoint(pName);
    resync();
}

void wxCapoc_Tree::OnSelect(wxTreeEvent &event) {
    procItem =  (wxCapoc_ItemData*) GetItemData(event.GetItem());

    switch(procItem->itemType)
    {
        case ITEM_POINT: {
            const std::string &name = procItem->pointName;
            capModel *model = engine->models[procItem->modelId];

            capGenericPoint &p = model->savedPoints[name];
            capAffineMatrix &m = model->affineMatrix;
            float v[3] = {p.x, p.y, p.z};
            m.transform(v);
            engine->setStatus("Point %s: model (%.2f %.2f %.2f), real (%.2f %.2f %.2f)", name.c_str(), p.x, p.y, p.z, v[0], v[1], v[2]);
            model->cursor.x = p.x; model->cursor.y = p.y; model->cursor.z = p.z;
            model->cursor.valid = true;
            engine->needRefresh = true;

            break;
        }
        case ITEM_MODEL: {
            capModel *m = engine->models[procItem->modelId];
            engine->setStatus("Model %d: %d points, %d triangles", procItem->modelId, m->nv, m->nt);
            break;
        }
        case ITEM_IMAGE: {
            capNvm *n = engine->models[0]->nvm;
            capNvmCam &cam = n->cams[procItem->camIndex];
            engine->setStatus("Image %s: %d sparse points seen, depth %.1f", cam.filename, cam.projCount, cam.depth);
            break;
        }
    }
}

void wxCapoc_Tree::OnActivate(wxTreeEvent &event) {
    auto * itemd =  (wxCapoc_ItemData*) GetItemData(event.GetItem());
    switch(itemd->itemType)
    {
        case ITEM_IMAGE: {
            engine->execStringf("model 0 nvm camera %d set-active\n", itemd->camIndex);
            engine->execStringf("view switch nvmcam\n");
            break;
        }
        case ITEM_POINT: {
            const std::string &name = procItem->pointName;
            engine->execStringf("model %d point %s see\n", itemd->modelId, name.c_str());
            engine->execStringf("view switch caver\n");
            break;
        }
    }
}

void wxCapoc_Tree::QuickMenuAction(wxCommandEvent &event) {
    unsigned int m = procItem->modelId;
    int id;
    switch((id = event.GetId()))
    {
        case ID_Sort_CamIndex: engine->execStringf("model 0 nvm camera order-by index\n"); resync_nvm(); break;
        case ID_Sort_Depth: engine->execStringf("model 0 nvm camera order-by depth\n"); resync_nvm(); break;
        case ID_Sort_Distance: engine->execStringf("model 0 nvm camera order-by distance\n"); resync_nvm(); break;
        case ID_Sort_Name: engine->execStringf("model 0 nvm camera order-by name\n"); resync_nvm(); break;
        case ID_Sort_Seen: engine->execStringf("model 0 nvm camera order-by seen\n"); resync_nvm(); break;
        case ID_Revert_Data: engine->execStringf("model %d load from %s\n", m, engine->models[m]->fullpath); resync(); break;
        case ID_Display_None: engine->execStringf("model %d display none\n", m); break;
        case ID_Display_Points: engine->execStringf("model %d display points\n", m); break;
        case ID_Display_Edges: engine->execStringf("model %d display edges\n", m); break;
        case ID_Display_Faces: engine->execStringf("model %d display faces\n", m); break;
        case ID_Selection_FitPlane: engine->execStringf("model %d selection fit-plane\n", m); break;
        case ID_Selection_Reset: engine->execStringf("model %d selection reset\n", m); break;
        case ID_Selection_Delete: engine->execStringf("model %d selection delete\n", m); break;
        case ID_Compute_Normals: engine->execStringf("model %d compute-normals\n", m); break;
        case ID_Set_Gray: engine->execStringf("model %d recolor 192 192 192 255\n", m); break;
        case ID_Transform_Reset: engine->execStringf("model %d transform matrix x 1 0 0 0 y 0 1 0 0 z 0 0 1 0\n", m); break;
        case ID_Cam_See:
            engine->execStringf("model 0 nvm camera %d see\n", procItem->camIndex);
            engine->execStringf("view switch caver\n");
            break;
        case ID_Cam_Recolor_Projectable: engine->execStringf("model 0 nvm camera %d reproject recolor-projectable\n", procItem->camIndex); break;
        case ID_Cam_Delete_Unprojectable: engine->execStringf("model 0 nvm camera %d reproject delete-unprojectable\n", procItem->camIndex); break;
        case ID_Cam_Gray_Unprojectable: engine->execStringf("model 0 nvm camera %d reproject gray-unprojectable\n", procItem->camIndex); break;
        case ID_Cam_Select_Projectable: engine->execStringf("model 0 nvm camera %d reproject mark-projectable\n", procItem->camIndex); break;
        case ID_Selection_Save:
        case ID_Save: {
            wxFileDialog saveFileDialog(this, id == ID_Save ? wxT("Save model") : wxT("Save selection"),
                                        "", "", "OFF files (*.off)|*.off", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
            if (saveFileDialog.ShowModal() == wxID_CANCEL) return;
            if(id == ID_Save)
                engine->models[m]->saveOff(saveFileDialog.GetPath().ToUTF8().data());
            else
                engine->models[m]->saveSelectionOff(saveFileDialog.GetPath().ToUTF8().data());
            break;
        }
        case ID_Append_Data:
        case ID_Replace_Data: {
            wxFileDialog openFileDialog(this, _("Open model file"), "", "",
                                            "All supported types (OFF, PLY, NVM, 3d)|*.off;*.ply;*.nvm;*.3d|OFF files (*.off)|*.off|PLY files (*.ply)|*.ply|NVM files (*.nvm)|*.nvm|3d files (*.3d)|*.3d", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
            if (openFileDialog.ShowModal() == wxID_CANCEL) return;
            if(id == ID_Append_Data)
                engine->models[m]->loadFile(openFileDialog.GetPath().ToUTF8().data(), true, NULL);
            else
                engine->models[m]->loadFile(openFileDialog.GetPath().ToUTF8().data(), false, NULL);
            break;
        }
        case ID_Transform_Matrix:
        {
            capAffineMatrix af(engine->models[m]->affineMatrix);
            wxCapoc_EditMatrix matrixDialog(&af, wxT("Edit transformation matrix"));
            if(matrixDialog.ShowModal() != wxID_OK) return;
            if(matrixDialog.composeCheckbox->IsChecked())
                engine->models[m]->affineMatrix.compose(&af);
            else
                engine->models[m]->affineMatrix.setFrom(&af);
            engine->needRefresh = true;
            break;
        }
        case ID_Set_Master: masterModel = m; resync_models(); break;
        case ID_Set_Edit: modelEditing = m; resync_models(); break;
        case ID_Cancel_Edit: modelEditing = -1; resync_models(); break;
        case ID_Resync: resync(); break;
        case ID_Transform_To:
        {
            if(masterModel >= engine->models.size())
                masterModel = 0;
            engine->execStringf("model %d transform to %d\n", m, masterModel);
            break;
        }

        case ID_NVM_Info:
            showLog(engine->execStringf("model 0 nvm camera %d diag info\n", procItem->camIndex), "NVM Camera Information");
            break;

        case ID_Info:
            showLog(engine->execStringf("model %d info\n", m), "Model information");
            break;

        default: break;
    }
}

wxCapoc_ItemData::wxCapoc_ItemData(unsigned int type) {
    itemType = type;
}

void wxCapoc_Tree::showLog(const char *log, const char *title)
{
    wxCapoc_LogBrowser logDialog(log, title);
    logDialog.ShowModal();
}