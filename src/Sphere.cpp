// Sphere.cpp

#include "stdafx.h"
#include "Sphere.h"
#include <BRepPrimAPI_MakeSphere.hxx>
#include <gp_Trsf.hxx>
#include "PropertyVertex.h"
#include "../interface/PropertyDouble.h"
#include "Gripper.h"
#include "MarkedList.h"

wxIcon* CSphere::m_icon = NULL;

CSphere::CSphere(const gp_Pnt& pos, double radius, const wxChar* title):m_pos(pos), m_radius(radius), CSolid(BRepPrimAPI_MakeSphere(pos, radius), title)
{
}

wxIcon* CSphere::GetIcon()
{
	if(m_icon == NULL){wxString exe_folder = wxGetApp().GetExeFolder();	m_icon = new wxIcon(exe_folder + _T("/icons/sphere.png"), wxBITMAP_TYPE_PNG);}
	return m_icon;
}

HeeksObj *CSphere::MakeACopy(void)const
{
	return new CSphere(*this);
}

static void on_set_centre(const gp_Pnt &vt, HeeksObj* object){
	((CSphere*)object)->m_pos = vt;
}

static void on_set_radius(double value, HeeksObj* object){
	((CSphere*)object)->m_radius = value;
}

void CSphere::ModifyByMatrix(const double* m, bool for_undo){
	if(!for_undo){
		gp_Trsf mat = make_matrix(m);
		gp_Pnt new_pos = m_pos.Transformed(mat);
		double scale = gp_Vec(1, 0, 0).Transformed(mat).Magnitude();
		double new_radius = fabs(m_radius * scale);
		wxGetApp().AddUndoably(new CSphere(new_pos, new_radius, m_title.c_str()), m_owner, NULL);
		wxGetApp().DeleteUndoably(this);
	}
}

void CSphere::GetProperties(std::list<Property *> *list)
{
	__super::GetProperties(list);

	list->push_back(new PropertyVertex(_T("centre"), m_pos, this, on_set_centre));
	list->push_back(new PropertyDouble(_T("radius"), m_radius, this, on_set_radius));
}

void CSphere::GetGripperPositions(std::list<double> *list, bool just_for_endof)
{
	list->push_back(GripperTypeTranslate);
	list->push_back(m_pos.X());
	list->push_back(m_pos.Y());
	list->push_back(m_pos.Z());
	list->push_back(GripperTypeScale);
	list->push_back(m_pos.X() + m_radius);
	list->push_back(m_pos.Y());
	list->push_back(m_pos.Z());
}

void CSphere::OnApplyProperties()
{
	CSphere* new_object = new CSphere(m_pos, m_radius, m_title.c_str());
	wxGetApp().StartHistory(_T("Edit Sphere"));
	wxGetApp().AddUndoably(new_object, NULL, NULL);
	wxGetApp().DeleteUndoably(this);
	wxGetApp().EndHistory();
	wxGetApp().m_marked_list->Clear();
	wxGetApp().m_marked_list->Add(new_object);
	wxGetApp().Repaint();
}

bool CSphere::GetCentrePoint(double* pos)
{
	extract(m_pos, pos);
	return true;
}

bool CSphere::GetScaleAboutMatrix(double *m)
{
	gp_Trsf mat;
	mat.SetTranslationPart(gp_Vec(m_pos.XYZ()));
	extract(mat, m);
	return true;
}