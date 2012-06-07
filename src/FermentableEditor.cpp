/*
 * FermentableEditor.cpp is part of Brewtarget, and is Copyright Philip G. Lee
 * (rocketman768@gmail.com), 2009-2011.
 *
 * Brewtarget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * Brewtarget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QIcon>
#include "FermentableEditor.h"
#include "fermentable.h"
#include "database.h"
#include "config.h"
#include "unit.h"
#include "brewtarget.h"

FermentableEditor::FermentableEditor( QWidget* parent )
        : QDialog(parent), obsFerm(0)
{
   setupUi(this);

   connect( this, SIGNAL( accepted() ), this, SLOT( save() ));
   connect( this, SIGNAL( rejected() ), this, SLOT( clearAndClose() ));
}

void FermentableEditor::setFermentable( Fermentable* f )
{
   if( obsFerm )
      disconnect( obsFerm, 0, this, 0 );
   
   obsFerm = f;
   if( obsFerm )
   {
      connect( obsFerm, SIGNAL(changed(QMetaProperty,QVariant)), this, SLOT(changed(QMetaProperty,QVariant)) );
      showChanges();
   }
}

void FermentableEditor::save()
{
   if( obsFerm == 0 )
   {
      setVisible(false);
      return;
   }

   // TODO: check this out with 1.2.5.
   // Need to disable notification since every "set" method will cause a "showChanges" that
   // will revert any changes made.
   //obsFerm->disableNotification();

   obsFerm->setName(lineEdit_name->text());
   //obsFerm->setType(comboBox_type->currentText());
   // NOTE: the following assumes that Fermentable::Type is enumerated in the same
   // order as the combobox.
   obsFerm->setType( static_cast<Fermentable::Type>(comboBox_type->currentIndex()) );
   obsFerm->setAmount_kg(Brewtarget::weightQStringToSI(lineEdit_amount->text()));
   obsFerm->setYield_pct(lineEdit_yield->text().toDouble());
   obsFerm->setColor_srm(Brewtarget::colorQStringToSI(lineEdit_color->text()));
   obsFerm->setAddAfterBoil( (checkBox_addAfterBoil->checkState() == Qt::Checked)? true : false );
   obsFerm->setOrigin( lineEdit_origin->text() );
   obsFerm->setSupplier( lineEdit_supplier->text() );
   obsFerm->setCoarseFineDiff_pct( lineEdit_coarseFineDiff->text().toDouble() );
   obsFerm->setMoisture_pct( lineEdit_moisture->text().toDouble() );
   obsFerm->setDiastaticPower_lintner( lineEdit_diastaticPower->text().toDouble() );
   obsFerm->setProtein_pct( lineEdit_protein->text().toDouble() );
   obsFerm->setMaxInBatch_pct( lineEdit_maxInBatch->text().toDouble() );
   obsFerm->setRecommendMash( (checkBox_recommendMash->checkState() == Qt::Checked) ? true : false );
   obsFerm->setIsMashed( (checkBox_isMashed->checkState() == Qt::Checked) ? true : false );
   obsFerm->setIbuGalPerLb( lineEdit_ibuGalPerLb->text().toDouble() );
   obsFerm->setNotes( textEdit_notes->toPlainText() );

   //obsFerm->reenableNotification();
   //obsFerm->forceNotify();

   setVisible(false);
}

void FermentableEditor::clearAndClose()
{
   setFermentable(0);
   setVisible(false); // Hide the window.
}

void FermentableEditor::changed(QMetaProperty prop, QVariant /*val*/)
{
   if( sender() == obsFerm )
      showChanges(&prop);
}

void FermentableEditor::showChanges(QMetaProperty* metaProp)
{
   if( obsFerm == 0 )
      return;

   QString propName;
   bool updateAll = false;
   if( metaProp == 0 )
      updateAll = true;
   else
   {
      propName = metaProp->name();
   }
   
   // Update the color label text.
   if (Brewtarget::getColorUnit() == Brewtarget::SRM)
      label_5->setText(QString("Color (Lovibond)"));
   else
      label_5->setText(QString("Color (EBC)"));

   if( propName == "name" || updateAll )
   {
      lineEdit_name->setText(obsFerm->name());
      lineEdit_name->setCursorPosition(0);
      if( ! updateAll )
         return;
   }
   if( propName == "type" || updateAll) {
      // NOTE: assumes the comboBox entries are in same order as Fermentable::Type
      comboBox_type->setCurrentIndex(obsFerm->type());
       if( ! updateAll )
         return;
   }
   if( propName == "amount_kg" || updateAll) {
      lineEdit_amount->setText(Brewtarget::displayAmount(obsFerm->amount_kg(), Units::kilograms));
      if( ! updateAll )
         return;
   }
   if( propName == "yield_pct" || updateAll) {
      lineEdit_yield->setText(Brewtarget::displayAmount(obsFerm->yield_pct(), 0));
      if( ! updateAll )
         return;
   }
   if( propName == "color_srm" || updateAll) {
      lineEdit_color->setText(Brewtarget::displayColor(obsFerm->color_srm(), noUnit, false));
       if( ! updateAll )
         return;
   }
   if( propName == "addAfterBoil" || updateAll) {
      checkBox_addAfterBoil->setCheckState( obsFerm->addAfterBoil() ? Qt::Checked : Qt::Unchecked );
       if( ! updateAll )
         return;
   }
   if( propName == "origin" || updateAll)
   {
      lineEdit_origin->setText(obsFerm->origin());
      lineEdit_origin->setCursorPosition(0);
      if( ! updateAll )
         return;
   }
   if( propName == "supplier" || updateAll)
   {
      lineEdit_supplier->setText(obsFerm->supplier());
      lineEdit_supplier->setCursorPosition(0);
       if( ! updateAll )
         return;
   }
   if( propName == "coarseFineDiff_pct" || updateAll) {
      lineEdit_coarseFineDiff->setText(Brewtarget::displayAmount(obsFerm->coarseFineDiff_pct(), 0));
      if( ! updateAll )
         return;
   }
   else if( propName == "moisture_pct" || updateAll) {
      lineEdit_moisture->setText(Brewtarget::displayAmount(obsFerm->moisture_pct(), 0));
      if( ! updateAll )
         return;
   }
   if( propName == "diastaticPower_lintner" || updateAll) {
      lineEdit_diastaticPower->setText(Brewtarget::displayAmount(obsFerm->diastaticPower_lintner(), 0));
      if( ! updateAll )
         return;
   }
   if( propName == "protein_pct" || updateAll) {
      lineEdit_protein->setText(Brewtarget::displayAmount(obsFerm->protein_pct(), 0));
      if( ! updateAll )
         return;
   }
   if( propName == "maxInBatch_pct" || updateAll) {
      lineEdit_maxInBatch->setText(Brewtarget::displayAmount(obsFerm->maxInBatch_pct(), 0));
      if( ! updateAll )
         return;
   }
   if( propName == "recommendMash" || updateAll) {
      checkBox_recommendMash->setCheckState( obsFerm->recommendMash() ? Qt::Checked : Qt::Unchecked );
      if( ! updateAll )
         return;
   }
   if( propName == "isMashed" || updateAll) {
      checkBox_isMashed->setCheckState( obsFerm->isMashed() ? Qt::Checked : Qt::Unchecked );
       if( ! updateAll )
         return;
   }
   if( propName == "ibuGalPerLb" || updateAll) {
      lineEdit_ibuGalPerLb->setText(Brewtarget::displayAmount(obsFerm->ibuGalPerLb(), 0));
      if( ! updateAll )
         return;
   }
   if( propName == "notes" || updateAll) {
      textEdit_notes->setPlainText( obsFerm->notes() );
      if( ! updateAll )
         return;
   }
}
